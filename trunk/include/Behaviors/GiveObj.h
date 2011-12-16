#ifndef GiveObj_H
#define	GiveObj_H

#include <string>
#include <Behaviors/Behavior.h>
#include <Core/BehaviourController.h>
#include <Core/Human.h>

using namespace CartWheel::Behaviors;

namespace CartWheel {
    class CartWheel3D;
    namespace Behaviors {

        class GiveObj : public Behavior {
        private:
            double nStep;
            int nTime;
            std::string sTargetObj;
            std::string sToHand;
            std::string sToHuman;
            Core::Human* human1;
            Core::Human* human2;
            Core::BehaviourController* bcontroller1;
            Core::BehaviourController* bcontroller2;
            bool bGiveObjFinished;
            
        protected:
            virtual void runStep();
            virtual void onInit();
            virtual void onFinish();

        public:
            GiveObj(CartWheel3D* cw, std::string humanName, GiveObj_Params* params);
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

