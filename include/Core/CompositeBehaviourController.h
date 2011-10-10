/*
 * CompositeBehaviourController.h
 *
 *  Created on: Feb 28, 2011
 *      Author: denis
 */

#ifndef COMPOSITEBEHAVIOURCONTROLLER_H_
#define COMPOSITEBEHAVIOURCONTROLLER_H_

#include <Core/BehaviourController.h>
#include <Core/IKVMCController.h>
#include <Core/WorldOracle.h>
#include <Core/IKVMCController.h>
#include <Core/Character.h>

namespace CartWheel {
    namespace Core {

        class CompositeBehaviourController : public BehaviourController {
        private:

            DynamicArray<BehaviourController*> controllers;
            DynamicArray<BehaviourController*> initialStateControllers;
            DynamicArray<double> behaviourDurations;
            DynamicArray<double> behaviourTransitions;

            BehaviourController* m_behaviour;
            int activeBehaviour;
            double timeElapsed;
            double timeTransitioned;
            bool loopBehaviours;
            bool processAllBehaviours;

            void addController(BehaviourController* behaviour, FILE* file = NULL);
            void switchToNextController(double dt);

        public:
            CompositeBehaviourController(Character* b, IKVMCController* llc, WorldOracle* w);
            ~CompositeBehaviourController();

            /**
                    Load controllers from the \param file handle.
             */
            virtual void loadFromFile(FILE * file);
            virtual void saveToFile(FILE * file);

            /**
                    a set of useful virtual functions, whose behavior can be overridden
             */
            virtual void setUpperBodyPose(double leanSagittal, double leanCoronal, double twist);
            virtual void setKneeBend(double v, bool swingAlso = false);
            virtual void setDuckWalkDegree(double v);
            virtual void setDesiredHeading(double v);
            virtual void setVelocities(double velDS, double velDC);

            virtual void adjustStepHeight();

            virtual void setElbowAngles(double leftElbowAngleX, double rightElbowAngleX, double leftElbowAngleY,
                                        double rightElbowAngleY, double leftElbowAngleZ, double rightElbowAngleZ);
            virtual void setHandAngles(double leftHandAngleX, double rightHandAngleX, double leftHandAngleY, 
                                        double rightHandAngleY, double leftHandAngleZ, double rightHandAngleZ);
            virtual void setShoulderAngles(double leftTwist, double rightTwist, double leftAdduction, double rightAdduction, double leftSwing, double rightSwing);
            virtual void setPelvisTorsoAngles(double pelvisAngleX, double pelvisAngleY, double pelvisAngleZ, 
                        double torsoAngleX, double torsoAngleY, double torsoAngleZ);
            virtual void setHeadAngles(double headAngleX, double headAngleY, double headAngleZ);

            virtual void requestStepTime(double stepTime);
            virtual void requestStepHeight(double stepHeight);
            virtual void requestVelocities(double velDS, double velDC);
            virtual void requestUpperBodyPose(double leanS, double leanC, double twist);
            virtual void requestKneeBend(double kb);
            virtual void requestDuckFootedness(double df);
            virtual void requestCoronalStepWidth(double corSW);
            virtual void requestElbowBend(double leftBendX, double rightBendX, double leftBendY, double rightBendY, double leftBendZ, double rightBendZ);
            virtual void requestHandBend(double leftHandAngleX, double rightHandAngleX, double leftHandAngleY, 
                        double rightHandAngleY, double leftHandAngleZ, double rightHandAngleZ);
            virtual void requestPelvisTorsoBend(double pelvisBendX, double pelvisBendY, double pelvisBendZ, 
                        double torsoBendX, double torsoBendY, double torsoBendZ);
            virtual void requestHeadBend(double headBendX, double headBendY, double headBendZ);
            virtual void requestShoulderAngles(LeftRightDouble shoulderTwist, LeftRightDouble shoulderCoronal, LeftRightDouble shoulderSagittal);
            /**
                    determines the desired swing foot location
             */
            virtual void setDesiredSwingFootLocation();

            /**
                    determine the estimate desired location of the swing foot, given the etimated position of the COM, and the phase
             */
            virtual CartWheel::Math::Vector3d computeSwingFootLocationEstimate(const CartWheel::Math::Point3d& comPos, double phase);

            /**
                    ask for a heading...
             */
            virtual void requestHeading(double v);

            /**
                    sets a bunch of parameters to some default initial value
             */
            virtual void initializeDefaultParameters();

            /**
                    this method gets called at every simulation time step
             */
            virtual void simStepPlan(double dt);

            /**
                    this method gets called every time the controller transitions to a new state
             */
            virtual void conTransitionPlan();

            /**
                    this method determines the degree to which the character should be panicking
             */
            virtual double getPanicLevel();
        };

    }
}

#endif /* COMPOSITEBEHAVIOURCONTROLLER_H_ */
