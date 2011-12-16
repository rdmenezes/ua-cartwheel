#ifndef Catch_H
#define	Catch_H

#include <string>
#include <Behaviors/Behavior.h>
#include <Core/BehaviourController.h>
#include <Core/Human.h>

using namespace CartWheel::Behaviors;

namespace CartWheel {
    class CartWheel3D;
    namespace Behaviors {

        class Catch : public Behavior {
        private:
            double nStep;
            int nTime;
            std::string sTargetObj;
            std::string sHand;
            Core::Human* human;
            Core::BehaviourController* bcontroller;
            bool bCatchFinished;
            
        protected:
            virtual void runStep();
            virtual void onInit();
            virtual void onFinish();

        public:
            Catch(CartWheel3D* cw, std::string humanName, Catch_Params* params);
            double getDistXZ(Point3d p1, Point3d p2);
            double getDistXYZ(Point3d p1, Point3d p2);
            double getBendOverK(Point3d pObj);
            void resetHumanAngles();
        };
    }
}

#endif

