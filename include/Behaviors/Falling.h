#ifndef Falling_H
#define	Falling_H

#include <string>
#include <Behaviors/Behavior.h>
#include <Core/Human.h>

using namespace CartWheel::Behaviors;

namespace CartWheel {
    class CartWheel3D;
    namespace Behaviors {

        class Falling : public Behavior {  
        private:            
            Core::Human* human;
            Core::BehaviourController* bcontroller;
            
        protected:
            virtual void runStep();
            virtual void onInit();
            virtual void onFinish();
            
        public:
            Falling(CartWheel3D* cw, std::string humanName, Falling_Params* params);
        };
    }
}

#endif

