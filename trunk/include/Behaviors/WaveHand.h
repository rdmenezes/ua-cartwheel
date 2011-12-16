#ifndef WaveHand_H
#define	WaveHand_H

#include <string>
#include <Behaviors/Behavior.h>
#include <Core/BehaviourController.h>
#include <Core/Human.h>

using namespace CartWheel::Behaviors;

namespace CartWheel {
    class CartWheel3D;
    namespace Behaviors {

        class WaveHand : public Behavior {
        private:
            int nFrame;
            std::string sHand;
            Core::Human* human;
            Core::BehaviourController* bcontroller;
            
        protected:
            virtual void runStep();
            virtual void onInit();
            virtual void onFinish();

        public:
            WaveHand(CartWheel3D* cw, std::string humanName, WaveHand_Params* params);
        };
    }
}

#endif

