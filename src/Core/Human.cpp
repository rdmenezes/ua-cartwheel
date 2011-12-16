#include <Core/Human.h>
#include <Core/ActionCollectionPolicy.h>

#include "Control/ExtendedAction.h"

using namespace CartWheel::GL;

using namespace CartWheel::Physics;
using namespace CartWheel::Math;
using namespace std;

#ifndef isnan
#define isnan(x) (x != x)
#define MUST_UNDEF_ISNAN
#endif

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
        bBusyBothHands(false),
        bBusyLeftHand(false),
        bBusyRightHand(false) {
            if (NULL != m_behaviour) {
                IKVMCController* ikvmccon = (dynamic_cast<IKVMCController*> (controller));
                if (NULL != ikvmccon) {
                    ikvmccon->setBehaviour(m_behaviour);
                    m_behaviour->conTransitionPlan();
                }
            }
            m_humanIK = new HumanoidIKCharacter(m_character);
            vMinPos = vMaxPos = Vector3d(SimGlobals::nan);
        }

        Human::Human(const std::string& name, Character* character,
                CompositeController* controller, Policy* policy) :
        m_name(name),
        m_character(character),
        m_controller(NULL),
        m_behaviour(NULL),
        m_policy(policy),
        m_composite_controller(controller),
        bBusyBothHands(false),
        bBusyLeftHand(false),
        bBusyRightHand(false) {
            m_humanIK = new HumanoidIKCharacter(m_character);
            vMinPos = vMaxPos = Vector3d(SimGlobals::nan);
        }

        Human::~Human() {
            delete m_character;
            delete m_controller;
            delete m_behaviour;
            delete m_policy;
            delete m_humanIK;
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
                if (speed > 8) {
                    speed = 8;
                }
                m_behaviour->requestVelocities(speed, 0);
                if (speed > 5) {
                    m_behaviour->requestStepHeight(0.025);
                }
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

        void Human::getLeftArmIK(Point3d pTarget, Vector3d* vShoulder, Vector3d* vElbow, Vector3d* vHand) {
            m_humanIK->setLeftArmTarget(pTarget, vShoulder, vElbow);
            m_humanIK->setLeftHandTarget(pTarget, vHand);
        }

        void Human::getRightArmIK(Point3d pTarget, Vector3d* vShoulder, Vector3d* vElbow, Vector3d* vHand) {
            m_humanIK->setRightArmTarget(pTarget, vShoulder, vElbow);
            m_humanIK->setRightHandTarget(pTarget, vHand);
        }

        void Human::getPelvisTorsoIK(Point3d pTarget, Vector3d* vPelvis, Vector3d* vTorso) {
            m_humanIK = new HumanoidIKCharacter(this->getCharacter());
            m_humanIK->setPelvisTorsoTarget(pTarget, vPelvis, vTorso);
        }

        void Human::getPelvisIK(Point3d pTarget, Vector3d* vPelvis) {
            m_humanIK = new HumanoidIKCharacter(this->getCharacter());
            m_humanIK->setPelvisTorsoTarget(pTarget, vPelvis, &Vector3d(0, 0, 0));
        }

        void Human::getHeadIK(Point3d pTarget, Vector3d* vHead) {
            //            printf("Moving headIK\n");
            m_humanIK = new HumanoidIKCharacter(this->getCharacter());
            m_humanIK->setHeadTarget(pTarget, vHead);
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

            if (!bBusyBothHands) {
                bool foundMatch = (m_grabJoints.find(targetName) != m_grabJoints.end());
                if (foundMatch) {
                    Joint* joint = m_grabJoints[targetName];
                    if (!odeWorld->removeJoint(joint))
                        printf("Error Trying to remove joint [%s] from ODEWorld\n.", targetName);

                    if (!m_character->removeJoint(joint))
                        printf("Error Trying to remove joint [%s] from Human class\n.", targetName);
                    m_grabJoints.erase(m_grabJoints.find(targetName));

                    ArticulatedRigidBody* attachmentHandL = m_character->getARBByName("lHand");
                    attachmentHandL->removeChildJoints();
                    ArticulatedRigidBody* attachmentHandR = m_character->getARBByName("rHand");
                    attachmentHandR->removeChildJoints();
                    bBusyLeftHand = false;
                    bBusyRightHand = false;
                }
            } else {
                std::string targetNameL = targetName;
                std::string targetNameR = targetName;
                ostringstream ostrT_L, ostrT_R;
                ostrT_L << targetNameL << "L";
                ostrT_R << targetNameR << "R";

                bool foundMatchL = (m_grabJoints.find(ostrT_L.str()) != m_grabJoints.end());
                bool foundMatchR = (m_grabJoints.find(ostrT_L.str()) != m_grabJoints.end());
                if (foundMatchL && foundMatchR) {
                    //Left Remove
                    Joint* jointL = m_grabJoints[ostrT_L.str()];
                    if (!odeWorld->removeJoint(jointL))
                        printf("Error Trying to remove joint [%s] from ODEWorld\n.", ostrT_L.str());
                    if (!m_character->removeJoint(jointL))
                        printf("Error Trying to remove joint [%s] from Human class\n.", ostrT_L.str());
                    m_grabJoints.erase(m_grabJoints.find(ostrT_L.str()));
                    ArticulatedRigidBody* attachmentHandL = m_character->getARBByName("lHand");
                    attachmentHandL->removeChildJoints();

                    //Right Remove
                    Joint* jointR = m_grabJoints[ostrT_R.str()];
                    if (!odeWorld->removeJoint(jointR))
                        printf("Error Trying to remove joint [%s] from ODEWorld\n.", ostrT_R.str());
                    if (!m_character->removeJoint(jointR))
                        printf("Error Trying to remove joint [%s] from Human class\n.", ostrT_R.str());
                    m_grabJoints.erase(m_grabJoints.find(ostrT_R.str()));
                    ArticulatedRigidBody* attachmentHandR = m_character->getARBByName("rHand");
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
            } else if (bBusyBothHands) {
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
                if (result==NULL)
                    printf("Not object to grab was found by name %s.\n", targetName.c_str());
            }

            ArticulatedRigidBody* attachmentHand = NULL;

            if (result) {
                switch (method) {
                    case left:
                        attachmentHand = m_character->getARBByName("lHand");
                        break;
                    case right:
                        attachmentHand = m_character->getARBByName("rHand");
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
                        childPos.x -= 0.045;
                        childPos.y -= 0.1;
                        childPos.z += 0.08;
                        parentJointPos = Point3d(0.09, -0.04, 0);
                        childJointPos = Point3d(0, 0, 0);
                        bBusyLeftHand = true;
                        break;
                    case right:
                        childPos.x += 0.045;
                        childPos.y -= 0.1;
                        childPos.z += 0.08;
                        parentJointPos = Point3d(-0.09, -0.04, 0);
                        childJointPos = Point3d(0, 0, 0);
                        bBusyRightHand = true;
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
                if (result==NULL)
                    printf("Not object to grab was found by name %s.\n", targetName.c_str());
            }

            ArticulatedRigidBody* attachmentHandL = m_character->getARBByName("lHand");
            ArticulatedRigidBody* attachmentHandR = m_character->getARBByName("rHand");
            result = (attachmentHandL != NULL && attachmentHandR != NULL);


            if (result) {
                //Left
                Point3d parentJointPosL, childJointPosL;
                //Right
                Point3d parentJointPosR, childJointPosR;

                // The local coordinate systems of the left and right limbs are rotated pi over y axis
                // Create offsets to make the hand cling to an object by the palm(s)
                //Left Positions
                parentJointPosL = Point3d(0.09, -0.04, 0);
                childJointPosL = Point3d(0, 0, 0);

                //Right Positions
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
                Point3d p = body->getCMPosition();
                body->setCMPosition(Point3d(p.x, p.y, p.z));
                odeWorld->setEngineStateFromRB();
                //                printf("\n\n----2nd Fix\n");
                odeWorld->addJoint(m_character, tempJointL);
                //                printf("\n\n----3rd Fix\n");
                odeWorld->addJoint(m_character, tempJointR);

                m_grabJoints[ostrT_L.str()] = tempJointL;
                m_grabJoints[ostrT_R.str()] = tempJointR;
            }
        }
        
        bool Human::getLeftHandBusy() {
            return bBusyLeftHand;
        }
        
        bool Human::getRightHandBusy() {
            return bBusyRightHand;            
        }
        
        bool Human::getBothHandsBusy() {
            return bBusyBothHands;            
        }
        
        bool Human::getFootTouchedGround() {            
            World& world = World::instance();
            ODEWorld* odeWorld = dynamic_cast<ODEWorld*> (&world);
            DynamicArray<ContactPoint> *cfs = odeWorld->getContactForces();
            string sGround("ground");
            string lFoot("lFoot");
            bool bFoot = false;
            
            //see if the feet touch the ground...
            for (unsigned int i = 0; i < cfs->size(); i++) {
                //if neither of the bodies involved are articulated, it means they are just props so we can ignore them
//                if (((*cfs)[i].rb1->isArticulated() == false && (*cfs)[i].rb2->isArticulated() == false))
//                    continue;      
                if (bFoot==false) {
                    string sRb1((*cfs)[i].rb1->getName());
                    bFoot = (m_controller->isStanceFoot((*cfs)[i].rb1) && sRb1.find("lFoot")!=string::npos);
                    bFoot = bFoot and (sGround.compare((*cfs)[i].rb2->getName())==0);
//                    if (bFoot)
//                        printf("Collision: %s, %s\n", (*cfs)[i].rb1->getName(), (*cfs)[i].rb2->getName());
                }
//                if (m_controller->isSwingFoot((*cfs)[i].rb1))
//                    printf("Collision: %s, %s\n", (*cfs)[i].rb1->getName(), (*cfs)[i].rb2->getName());
//                else
//                    printf("Collision: %s, %s\n", (*cfs)[i].rb1->getName(), (*cfs)[i].rb2->getName());
                
//                if (sGround.compare((*cfs)[i].rb2->getName())==0)
//                    printf("Collision: %s, %s\n", (*cfs)[i].rb1->getName(), (*cfs)[i].rb2->getName());
//                if ((m_controller->isStanceFoot((*cfs)[i].rb1) && sGround.compare((*cfs)[i].rb2->getName())==0) ||
//                        (m_controller->isStanceFoot((*cfs)[i].rb2) && sGround.compare((*cfs)[i].rb1->getName())==0))
//                    return true;
            }
            return bFoot;
            
//            if (bFoot==false)
//                printf("No Foot collision\n");
//            return false;
        }

        

        void Human::setLock(const std::string& sARB, bool bLockPosition, bool bLockOrientation) {
            if(bLockPosition)
                setLockPosition(sARB.c_str(), true, true, true);
            else
                setLockPosition(sARB.c_str(), false, false, false);
            
            if(bLockOrientation)
                setLockOrientation(sARB.c_str(), true, true, true);
            else
                setLockOrientation(sARB.c_str(), false, false, false);
        }
        
        void Human::setLock(const std::string& sARB, bool bPosX, bool bPosY, bool bPosZ, bool bAngleX, bool bAngleY, bool bAngleZ) {
            setLockPosition(sARB.c_str(), bPosX, bPosY, bPosZ);
            setLockOrientation(sARB.c_str(), bAngleX, bAngleY, bAngleZ);
        }

        void Human::setLockPosition(const std::string& sARB, bool bLockX, bool bLockY, bool bLockZ) {
            Vector3d vSpeed = m_character->getARBByName(sARB.c_str())->getCMVelocity();
            vSpeed.x = bLockX ? 0 : vSpeed.x;
            vSpeed.y = bLockY ? 0 : vSpeed.y;
            vSpeed.z = bLockZ ? 0 : vSpeed.z;
            
            this->setVelocity(sARB.c_str(), vSpeed);
        }

        void Human::setLockOrientation(const std::string& sARB, bool bLockX, bool bLockY, bool bLockZ) {
            Vector3d vAngle = m_character->getARBByName(sARB.c_str())->getOrientation().getAngles();
            vAngle.x = bLockX ? 0 : vAngle.x;
            vAngle.y = bLockY ? 0 : vAngle.y;
            vAngle.z = bLockZ ? 0 : vAngle.z;
            
            this->setAngVelocity(sARB.c_str(), vAngle);
        }

        void Human::setPosition(const std::string& sARB, Vector3d vPos, bool bIncremental) {
            Vector3d vInitPos(0, 0, 0);
            if (bIncremental) {
                vInitPos = m_character->getARBByName(sARB.c_str())->getCMPosition();
                vPos = vInitPos + vPos;
            }
            //Max Limit
            vPos.x = !isnan(vMaxPos.x) && (vPos.x > vMaxPos.x) ? vMaxPos.x : vPos.x;
            vPos.y = !isnan(vMaxPos.y) && (vPos.y > vMaxPos.y) ? vMaxPos.y : vPos.y;
            vPos.z = !isnan(vMaxPos.z) && (vPos.z > vMaxPos.z) ? vMaxPos.z : vPos.z;

            //Min Limit
            vPos.x = !isnan(vMinPos.x) && (vPos.x < vMinPos.x) ? vMinPos.x : vPos.x;
            vPos.y = !isnan(vMinPos.y) && (vPos.y < vMinPos.y) ? vMinPos.y : vPos.y;
            vPos.z = !isnan(vMinPos.z) && (vPos.z < vMinPos.z) ? vMinPos.z : vPos.z;
            

            m_character->getARBByName(sARB.c_str())->setCMPosition(vPos);
        }

        void Human::setOrientation(const std::string& sARB, Vector3d vAngle, bool bIncremental) {            
            Quaternion qInitAngle;
            if (bIncremental) {
                qInitAngle = m_character->getARBByName(sARB.c_str())->getOrientation();
            }
            Quaternion qAngle = qInitAngle
                    *Quaternion::getRotationQuaternion(vAngle.x, Vector3d(1,0,0))
                    *Quaternion::getRotationQuaternion(vAngle.y, Vector3d(0,1,0))
                    *Quaternion::getRotationQuaternion(vAngle.z, Vector3d(0,0,1));
            
            m_character->getARBByName(sARB.c_str())->setOrientation(qAngle);
        }

        void Human::setConstraintPos(Vector3d vMinPos, Vector3d vMaxPos) {
            this->vMinPos = vMinPos;
            this->vMaxPos = vMaxPos;
        }

        void Human::setExternalForce(const std::string& sARB, Vector3d vForce, bool bIncremental) {
            Vector3d vInitForce(0, 0, 0);
            if (bIncremental) {
                vInitForce = m_character->getARBByName(sARB.c_str())->getExternalForce();
                vForce = vInitForce + vForce;
            }
            m_character->getARBByName(sARB.c_str())->setExternalForce(vForce);
        }

        void Human::setExternalTorque(const std::string& sARB, Vector3d vTorque, bool bIncremental) {
            Vector3d vInitTorque(0, 0, 0);
            if (bIncremental) {
                vInitTorque = m_character->getARBByName(sARB.c_str())->getExternalTorque();
                vTorque = vInitTorque + vTorque;
            }
            m_character->getARBByName(sARB.c_str())->setExternalTorque(vTorque);
        }

        void Human::setVelocity(const std::string& sARB, Vector3d vSpeed, bool bIncremental) {
            Vector3d vInitSpeed(0, 0, 0);
            if (bIncremental) {
                vInitSpeed = m_character->getARBByName(sARB.c_str())->getCMVelocity();
                vSpeed = vInitSpeed + vSpeed;
            }
            m_character->getARBByName(sARB.c_str())->setCMVelocity(vSpeed);
        }

        void Human::setAngVelocity(const std::string& sARB, Vector3d vAngSpeed, bool bIncremental) {
            Vector3d vInitAngSpeed(0, 0, 0);
            if (bIncremental) {
                vInitAngSpeed = m_character->getARBByName(sARB.c_str())->getAngularVelocity();
                vAngSpeed = vInitAngSpeed + vAngSpeed;
            }
            m_character->getARBByName(sARB.c_str())->setAngularVelocity(vAngSpeed);
        }
        
        void Human::factorVelocity(const std::string& sARB, Vector3d vFactor) {
            Vector3d vSpeed = m_character->getARBByName(sARB.c_str())->getCMVelocity();
            vSpeed.x = vFactor.x * vSpeed.x;
            vSpeed.y = vFactor.y * vSpeed.y;
            vSpeed.z = vFactor.z * vSpeed.z;
            
            this->setVelocity(sARB.c_str(), vSpeed);
        }
        
        void Human::factorAngVelocity(const std::string& sARB, Vector3d vFactor) {
            Vector3d vAngSpeed = m_character->getARBByName(sARB.c_str())->getAngularVelocity();
            vAngSpeed.x = vFactor.x * vAngSpeed.x;
            vAngSpeed.y = vFactor.y * vAngSpeed.y;
            vAngSpeed.z = vFactor.z * vAngSpeed.z;
            
            this->setAngVelocity(sARB.c_str(), vAngSpeed);
        }
        
        void Human::setOrientationQ(const std::string& sARB, Quaternion qAngle) {
            m_character->getARBByName(sARB.c_str())->setOrientation(qAngle);
        }
        
        Quaternion Human::getOrientationQ(const std::string& sARB) {
            return m_character->getARBByName(sARB.c_str())->getOrientation();
        }
    }
}

#ifdef MUST_UNDEF_ISNAN
#undef isnan(x) (x != x)
#undef MUST_UNDEF_ISNAN
#endif