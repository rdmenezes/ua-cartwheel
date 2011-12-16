#ifndef WalkInPath_H
#define	WalkInPath_H

#include <string>
#include <Behaviors/Behavior.h>
#include <Core/BehaviourController.h>
#include <Core/Human.h>
#include <GLUtils/GLUtils.h>
#include <MathLib/Trajectory.h>

using namespace CartWheel::Behaviors;
using namespace CartWheel::Math;

namespace CartWheel {
    class CartWheel3D;
    namespace Behaviors {

        class WalkInPath : public Behavior {
        private:
            double nAngSpeed;
            double nStep;
            int nTime;
            int nPoint;
            Trajectory3d* tPath;
            Core::Human* human;
            Core::BehaviourController* bcontroller;
            
        protected:
            virtual void runStep();
            virtual void onInit();
            virtual void onFinish();

        public:
            WalkInPath(CartWheel3D* cw, std::string humanName, WalkInPath_Params* params);
            double getAngle(Point3d p3);
            Point3d getPathPoint(double time);
            double getDistance(int nP);
            double getTimeLeft(int nP, double time);
        };
    }
}

#endif

