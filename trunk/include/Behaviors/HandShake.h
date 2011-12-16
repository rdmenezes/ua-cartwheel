#ifndef HandShake_H
#define	HandShake_H

#include <string>
#include <Behaviors/Behavior.h>
#include <Core/BehaviourController.h>
#include <Core/Human.h>

using namespace CartWheel::Behaviors;

namespace CartWheel {
    class CartWheel3D;
    namespace Behaviors {

        class HandShake : public Behavior {
        private:
            double nStep;
            int nTime;
            int nFrame;
            std::string sFromHand;
            std::string sToHand;
            std::string sToHuman;
            Core::Human* human1;
            Core::Human* human2;
            Core::BehaviourController* bcontroller1;
            Core::BehaviourController* bcontroller2;
            bool bHandShakeFinished;
            
        protected:
            virtual void runStep();
            virtual void onInit();
            virtual void onFinish();

        public:
            HandShake(CartWheel3D* cw, std::string humanName, HandShake_Params* params);
            double getDistXZ(Point3d p1, Point3d p2);
            double getDistXYZ(Point3d p1, Point3d p2);
            double getBendOverK(Point3d pObj);
            Point3d moveIK_Human1();
            Point3d moveIK_Human2();
            void resetHumanAngles();
        };
    }
}

#endif

