#include <Core/Human.h>
#include <Core/ActionCollectionPolicy.h>

using namespace CartWheel::GL;

using namespace CartWheel::Physics;
using namespace CartWheel::Math;
using namespace std;

namespace CartWheel {
    namespace Core {

        Human::Human(const std::string& name, Character* character,
                SimBiController* controller, BehaviourController* behaviour,
                Policy* policy) :
        m_name(name),
        m_character(character),
        m_controller(controller),
        m_behaviour(behaviour),
        m_policy(policy),
        m_composite_controller(NULL),
        bBusyBothHands(false) {
            if (NULL != m_behaviour) {
                IKVMCController* ikvmccon = (dynamic_cast<IKVMCController*> (controller));
                if (NULL != ikvmccon) {
                    ikvmccon->setBehaviour(m_behaviour);
                    m_behaviour->conTransitionPlan();
                }
            }
        }

        Human::Human(const std::string& name, Character* character,
                CompositeController* controller, Policy* policy) :
        m_name(name),
        m_character(character),
        m_controller(NULL),
        m_behaviour(NULL),
        m_policy(policy),
        m_composite_controller(controller),
        bBusyBothHands(false) {
        }

        Human::~Human() {
            delete m_character;
            delete m_controller;
            delete m_behaviour;
            delete m_policy;
        }

        void Human::init() {
            applyAction(0);
        }

        void Human::setCharacter(CartWheel::Core::Character* character) {
            m_character = character;
        }

        void Human::setController(CartWheel::Core::SimBiController* controller) {
            m_controller = controller;
        }

        void Human::setBehaviour(CartWheel::Core::BehaviourController* behaviour) {
            m_behaviour = behaviour;
        }

        void Human::setPolicy(CartWheel::Core::Policy* policy) {
            m_policy = policy;
        }

        void Human::setPosition(const CartWheel::Math::Point3d& position) {
            m_character->setPos(position);
        }

        void Human::setHeading(double angle) {
            //	m_character->setHeading(angle);

            // If there is a behavior associated with the controller
            // ask it for a desired heading
            // If all else fails, set the heading on the controller directly.
            if (NULL != m_behaviour) {
                m_behaviour->requestHeading(angle);
                m_behaviour->conTransitionPlan();
            } else if (NULL != m_controller) {
                m_controller->setDesiredHeading(angle);
            }
        }

        void Human::setSpeed(double speed) {
            if (NULL != m_behaviour) {
                m_behaviour->requestVelocities(speed, 0);
                m_behaviour->conTransitionPlan();
            }
        }

        void Human::setStepWidth(double width) {
            if (NULL != m_behaviour) {
                m_behaviour->requestCoronalStepWidth(width);
                m_behaviour->conTransitionPlan();
            }
        }

        void Human::setName(const std::string& name) {
            m_name = name;
        }

        void Human::applyAction(int actionIndex) {
            ActionCollectionPolicy* policy = dynamic_cast<ActionCollectionPolicy*> (m_policy);

            if (NULL != policy) {
                double heading = getHeading();

                policy->setActionIndex(actionIndex);
                policy->applyAction();

                SimBiController* controller = m_composite_controller->getController(actionIndex);

                setController(controller);

                BehaviourController* behaviour = NULL;

                IKVMCController* ikvmccon = (dynamic_cast<IKVMCController*> (controller));

                if (NULL != ikvmccon) {
                    behaviour = ikvmccon->getBehaviour();
                }

                setBehaviour(behaviour);

                // Maintain the current heading
                setHeading(heading);
            }
        }

        void Human::dropObject(const std::string& targetName) {
            World& world = World::instance();
            ODEWorld* odeWorld = dynamic_cast<ODEWorld*> (&world);

            if(!bBusyBothHands) {
                bool foundMatch = (m_grabJoints.find(targetName) != m_grabJoints.end());
                if (foundMatch) {
                    Joint* joint = m_grabJoints[targetName];
                    if(!odeWorld->removeJoint(joint))
                        printf("Error Trying to remove joint [%s] from ODEWorld\n.", targetName);

                    if(!m_character->removeJoint(joint))
                        printf("Error Trying to remove joint [%s] from Human class\n.", targetName);
                    m_grabJoints.erase(m_grabJoints.find(targetName));
                    
                    ArticulatedRigidBody* attachmentHandL = m_character->getARBByName("lLowerArm");
                    attachmentHandL->removeChildJoints();
                    ArticulatedRigidBody* attachmentHandR = m_character->getARBByName("rLowerArm");
                    attachmentHandR->removeChildJoints();
                }
            } else {
                std::string targetNameL = targetName;
                std::string targetNameR = targetName;
                ostringstream ostrT_L, ostrT_R;
                ostrT_L << targetNameL << "L";
                ostrT_R << targetNameR << "R";   
                
                bool foundMatchL = (m_grabJoints.find(ostrT_L.str()) != m_grabJoints.end());
                bool foundMatchR = (m_grabJoints.find(ostrT_L.str()) != m_grabJoints.end());
                if(foundMatchL && foundMatchR) {
                    //Left Remove
                    Joint* jointL = m_grabJoints[ostrT_L.str()];                    
                    if(!odeWorld->removeJoint(jointL))
                        printf("Error Trying to remove joint [%s] from ODEWorld\n.", ostrT_L.str());
                    if(!m_character->removeJoint(jointL))
                        printf("Error Trying to remove joint [%s] from Human class\n.", ostrT_L.str());
                    m_grabJoints.erase(m_grabJoints.find(ostrT_L.str()));
                    ArticulatedRigidBody* attachmentHandL = m_character->getARBByName("lLowerArm");
                    attachmentHandL->removeChildJoints();
                    
                    //Right Remove
                    Joint* jointR = m_grabJoints[ostrT_R.str()];
                    if(!odeWorld->removeJoint(jointR))
                        printf("Error Trying to remove joint [%s] from ODEWorld\n.", ostrT_R.str());
                    if(!m_character->removeJoint(jointR))
                        printf("Error Trying to remove joint [%s] from Human class\n.", ostrT_R.str());
                    m_grabJoints.erase(m_grabJoints.find(ostrT_R.str()));
                    ArticulatedRigidBody* attachmentHandR = m_character->getARBByName("rLowerArm");
                    attachmentHandR->removeChildJoints();
                    
                    
                    bBusyBothHands = false;
                }
            }

            bool foundMatch = (m_grabbedBodies.find(targetName) != m_grabbedBodies.end());
            if (foundMatch) {
                ArticulatedRigidBody* body = m_grabbedBodies[targetName];
                body->setParentJoint(NULL);
                body->setAFParent(NULL);

                m_grabbedBodies.erase(m_grabbedBodies.find(targetName));
            }
        }

        void Human::throwObject(const std::string& targetName, const Vector3d& velocity) {
            bool foundMatch = (m_grabbedBodies.find(targetName) != m_grabbedBodies.end());
            if (foundMatch) {
                m_grabbedBodies[targetName]->setCMVelocity(velocity);
            }
            dropObject(targetName);
        }

        void Human::grabObject(const std::string& targetName, const GrabbingMethod& method) {
            if (method == both && !bBusyBothHands) {
                grabObject_BothHands(targetName);
                bBusyBothHands = true;
                return;
            } else if(bBusyBothHands) {                
                return;
            }

            World& world = World::instance();
            ODEWorld* odeWorld = dynamic_cast<ODEWorld*> (&world);

            RigidBody* rigidBody = world.getRBByName(targetName.c_str());
            ArticulatedRigidBody* body = NULL;
            bool foundMatch = (m_grabbedBodies.find(targetName) != m_grabbedBodies.end());
            bool result = false;

            if (foundMatch == false) {
                m_grabbedBodies[targetName] = dynamic_cast<ArticulatedRigidBody*> (rigidBody);
                body = m_grabbedBodies[targetName];
                result = (NULL != body);
            }

            ArticulatedRigidBody* attachmentHand = NULL;

            if (result) {
                switch (method) {
                    case left:
                        attachmentHand = m_character->getARBByName("lLowerArm");
                        break;
                    case right:
                        attachmentHand = m_character->getARBByName("rLowerArm");
                        break;
                }
                result = (NULL != attachmentHand);
            }

            if (result) {
                Point3d parentPos = attachmentHand->getCMPosition();
                Point3d childPos = parentPos;
                Point3d parentJointPos;
                Point3d childJointPos;

                // The local coordinate systems of the left and right limbs are rotated pi over y axis
                // Create offsets to make the hand cling to an object by the palm(s)
                switch (method) {
                    case left:
                        childPos.x -= 0.04;
                        childPos.y -= 0.07;
                        parentJointPos = Point3d(0.09, -0.04, 0);
                        childJointPos = Point3d(0, 0, 0);
                        break;
                    case right:
                        childPos.x += 0.04;
                        childPos.y -= 0.07;
                        parentJointPos = Point3d(-0.09, -0.04, 0);
                        childJointPos = Point3d(0, 0, 0);
                        break;
                }


                HingeJoint* tempJoint = new HingeJoint();

                ostringstream ostr;
                ostr << "ext_hand2" << targetName;
                string name = ostr.str();

                tempJoint->setName(name.c_str());
                tempJoint->setAxis(Vector3d(0, 1, 0));
                tempJoint->setParentJointPosition(parentJointPos);
                tempJoint->setChildJointPosition(childJointPos);

                // Setting high stop angle < low stop angle, effectively disables them, making the joint stiffer?
                tempJoint->setJointLimits(0, 0);
                tempJoint->setChild(body);
                tempJoint->setParent(attachmentHand);

                m_character->addJoint(tempJoint);
                body->setAFParent(m_character);

                m_character->fixJointConstraints(true, true);
                body->setCMPosition(childPos);
                odeWorld->setEngineStateFromRB();
                odeWorld->addJoint(m_character, tempJoint);

                m_grabJoints[targetName] = tempJoint;
            }
        }

        void Human::grabObject_BothHands(const std::string& targetName) {
            World& world = World::instance();
            ODEWorld* odeWorld = dynamic_cast<ODEWorld*> (&world);

            RigidBody* rigidBody = world.getRBByName(targetName.c_str());
            ArticulatedRigidBody* body = NULL;
            std::string targetNameL = targetName;
            std::string targetNameR = targetName;
            ostringstream ostrT_L, ostrT_R;
            ostrT_L << targetNameL << "L";
            ostrT_R << targetNameR << "R";            
            bool foundMatch = (m_grabbedBodies.find(targetName) != m_grabbedBodies.end());
            bool result = false;

            if (foundMatch == false) {                
                m_grabbedBodies[targetName] = dynamic_cast<ArticulatedRigidBody*> (rigidBody);
                body = m_grabbedBodies[targetName];
                result = (NULL != body);
            }

            ArticulatedRigidBody* attachmentHandL = m_character->getARBByName("lLowerArm");
            ArticulatedRigidBody* attachmentHandR = m_character->getARBByName("rLowerArm");
            result = (attachmentHandL!=NULL && attachmentHandR!=NULL);


            if (result) {
                //Left
                Point3d parentPosL = attachmentHandL->getCMPosition();
                Point3d childPosL = parentPosL;
                Point3d parentJointPosL, childJointPosL;
                
                //Right
                Point3d parentPosR = attachmentHandR->getCMPosition();
                Point3d childPosR = parentPosR;
                Point3d parentJointPosR, childJointPosR;

                // The local coordinate systems of the left and right limbs are rotated pi over y axis
                // Create offsets to make the hand cling to an object by the palm(s)
                //Left Positions
                childPosL.x -= 0.04;
                childPosL.y -= 0.07;
                parentJointPosL = Point3d(0.09, -0.04, 0);
                childJointPosL = Point3d(0, 0, 0);

                //Right Positions
                childPosR.x += 0.04;
                childPosR.y -= 0.07;
                parentJointPosR = Point3d(-0.09, -0.04, 0);
                childJointPosR = Point3d(0, 0, 0);


                //Left Joint
                HingeJoint* tempJointL = new HingeJoint();

                ostringstream ostrL;
                ostrL << "ext_lhand2" << targetName;
                string name = ostrL.str();

                tempJointL->setName(name.c_str());
                tempJointL->setAxis(Vector3d(0, 1, 0));
                tempJointL->setParentJointPosition(parentJointPosL);
                tempJointL->setChildJointPosition(childJointPosL);

                // Setting high stop angle < low stop angle, effectively disables them, making the joint stiffer?
                tempJointL->setJointLimits(0, 0);
                tempJointL->setChild(body);
                tempJointL->setParent(attachmentHandL);
                
                
                //Right Joint
                HingeJoint* tempJointR = new HingeJoint();

                ostringstream ostrR;
                ostrR << "ext_rhand2" << targetName;
                name = ostrR.str();

                tempJointR->setName(name.c_str());
                tempJointR->setAxis(Vector3d(0, 1, 0));
                tempJointR->setParentJointPosition(parentJointPosR);
                tempJointR->setChildJointPosition(childJointPosR);

                // Setting high stop angle < low stop angle, effectively disables them, making the joint stiffer?
                tempJointR->setJointLimits(0, 0);
                tempJointR->setParent(attachmentHandR);
                tempJointR->setChild2(body);
                

                m_character->addJoint(tempJointL);
                m_character->addJoint(tempJointR);
                body->setAFParent(m_character);

//                printf("----1st Fix\n");
                m_character->fixJointConstraints(true, true);
                Vector3d p = m_character->getCOM();
                body->setCMPosition(Point3d(p.x, p.y, p.z+0.2));
                odeWorld->setEngineStateFromRB();
//                printf("\n\n----2nd Fix\n");
                odeWorld->addJoint(m_character, tempJointL);
//                printf("\n\n----3rd Fix\n");
                odeWorld->addJoint(m_character, tempJointR);

                m_grabJoints[ostrT_L.str()] = tempJointL;
                m_grabJoints[ostrT_R.str()] = tempJointR;
            }
        }

    }
}
