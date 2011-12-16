#ifndef HUMAN_H_
#define HUMAN_H_

#include <map>
#include <string>

#include <MathLib/Vector3d.h>
#include <MathLib/Quaternion.h>
#include <MathLib/Point3d.h>
#include <MathLib/TransformationMatrix.h>

#include <Core/BehaviourController.h>
#include <Core/SimBiController.h>
#include <Core/HumanoidIKCharacter.h>
#include <Core/WorldOracle.h>
#include <Core/CompositeController.h>
#include <Core/Policy.h>

#include <Physics/CollisionDetectionPrimitive.h>
#include <Physics/CapsuleCDP.h>
#include <Physics/Joint.h>
#include <Physics/ArticulatedRigidBody.h>

using namespace CartWheel::Math;
using namespace CartWheel::Core;

namespace CartWheel {

    namespace Core {

        class Human {
        private:
            std::string m_name;
            Character* m_character;
            SimBiController* m_controller;
            BehaviourController* m_behaviour;
            Policy* m_policy;
            CompositeController* m_composite_controller;
            std::map<std::string, Physics::Joint*> m_grabJoints;
            std::map<std::string, Physics::ArticulatedRigidBody*> m_grabbedBodies;
            HumanoidIKCharacter* m_humanIK;
            bool bBusyBothHands, bBusyLeftHand, bBusyRightHand;
            Vector3d vMinPos, vMaxPos;
            

            Human() {
            }

        public:

            Human(const std::string& name, Character* character, SimBiController* controller,
                    BehaviourController* behaviour = NULL, Policy* policy = NULL);
            Human(const std::string& name, Character* character, CompositeController* controller, Policy* policy);

            virtual ~Human();

            inline Character* getCharacter() {
                return m_character;
            }

            inline SimBiController* getController() {
                return m_controller;
            }

            inline CompositeController* getCompositeController() {
                return m_composite_controller;
            }

            inline BehaviourController* getBehaviour() {
                return m_behaviour;
            }

            inline Policy* getPolicy() {
                return m_policy;
            }

            inline std::string getName() {
                return m_name;
            }

            inline Vector3d getPosition() {
                return m_character->getCOM();
            }

            inline double getHeading() {
                return m_character->getHeadingAngle();
            }

            inline Vector3d getVelocity() {
                return m_character->getCOMVelocity();
            }
            
            inline Vector3d getPosition(std::string sARB) {
                return m_character->getARBByName(sARB.c_str())->getCMPosition();
            }
            
            bool getLeftHandBusy();
            bool getRightHandBusy();
            bool getBothHandsBusy();
            
            void getLeftArmIK(Point3d pTarget, Vector3d* vShoulder, Vector3d* vElbow, Vector3d* vHand);
            void getRightArmIK(Point3d pTarget, Vector3d* vShoulder, Vector3d* vElbow, Vector3d* vHand);
            void getPelvisTorsoIK(Point3d pTarget, Vector3d* vPelvis, Vector3d* vTorso);
            void getHeadIK(Point3d pTarget, Vector3d* vHead);
            void getPelvisIK(Point3d pTarget, Vector3d* vPelvis);

            void setCharacter(Character* character);
            void setController(SimBiController* controller);
            void setBehaviour(BehaviourController* behaviour);
            void setPolicy(Policy* policy);
            void setPosition(const Point3d& position);

            /// @param angle Angle in radians
            void setHeading(double angle);

            void setSpeed(double speed);
            void setStepWidth(double width);
            void setName(const std::string& name);
            
            bool getFootTouchedGround();
            void setLock(const std::string& sARB, bool bLockPosition = false, bool bLockOrientation = false);
            void setLock(const std::string& sARB, bool bPosX, bool bPosY, bool bPosZ, 
                bool bAngleX = false, bool bAngleY = false, bool bAngleZ = false);
            void setLockPosition(const std::string& sARB, bool bLockX = false, bool bLockY = false, bool bLockZ = false);
            void setLockOrientation(const std::string& sARB, bool bLockX = false, bool bLockY = false, bool bLockZ = false);
            void setPosition(const std::string& sARB, Vector3d vPos, bool bIncremental = false);
            void setOrientation(const std::string& sARB, Vector3d vAngle, bool bIncremental = false);
            void setConstraintPos(Vector3d minPos, Vector3d maxPos);
            void setConstraintOrientation(Vector3d minAngle, Vector3d maxAngle);
            void setExternalForce(const std::string& sARB, Vector3d vForce, bool bIncremental = false);
            void setExternalTorque(const std::string& sARB, Vector3d vTorque, bool bIncremental = false);
            void setVelocity(const std::string& sARB, Vector3d vSpeed, bool bIncremental = false);
            void setAngVelocity(const std::string& sARB, Vector3d vAngSpeed, bool bIncremental = false);
            void setOrientationQ(const std::string& sARB, Quaternion qAngle);
            Quaternion getOrientationQ(const std::string& sARB);
            void factorVelocity(const std::string& sARB, Vector3d vFactor);
            void factorAngVelocity(const std::string& sARB, Vector3d vFactor);
            

            /**
             * Initialize everything.
             */
            void init();

            /**
             * Apply new action based on the index as prescribed in ActionCollectionPolicy.
             */
            void applyAction(int actionIndex);

            enum GrabbingMethod {
                left,
                right,
                both
            };

            void grabObject(const std::string& targetName, const GrabbingMethod& method);
            void grabObject_BothHands(const std::string& targetName);
            void dropObject(const std::string& targetName);
            void throwObject(const std::string& targetName, const Vector3d& velocity);
        };

    }

}

#endif /* HUMAN_H_ */
