#ifndef Jump_H
#define	Jump_H

#include <string>
#include <Behaviors/Behavior.h>
#include <Core/BehaviourController.h>
#include <Core/Human.h>

using namespace CartWheel::Behaviors;

namespace CartWheel {
    class CartWheel3D;
    namespace Behaviors {

        class Jump : public Behavior {
        private:
            int nFrame;
            double nDuration;
            double strength;
            std::string sDirection;
            Core::Human* human;
            Core::BehaviourController* bcontroller;
            
        protected:
            virtual void runStep();
            virtual void onInit();
            virtual void onFinish();
            
        public:
            Jump(CartWheel3D* cw, std::string humanName, Jump_Params* params);
        };
    }
}

#endif

