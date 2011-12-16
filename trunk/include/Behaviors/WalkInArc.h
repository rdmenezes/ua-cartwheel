#ifndef WalkInArc_H
#define	WalkInArc_H

#include <string>
#include <Behaviors/Behavior.h>
#include <Core/BehaviourController.h>
#include <Core/Human.h>

using namespace CartWheel::Behaviors;

namespace CartWheel {
    class CartWheel3D;
    namespace Behaviors {

        class WalkInArc : public Behavior {
        private:
            double nSpeed;
            double nAngSpeed;
            double nStep;
            int nTime;
            Core::Human* human;
            Core::BehaviourController* bcontroller;
            
        protected:
            virtual void runStep();
            virtual void onInit();
            virtual void onFinish();

        public:
            WalkInArc(CartWheel3D* cw, std::string humanName, WalkInArc_Params* params);
        };
    }
}

#endif

