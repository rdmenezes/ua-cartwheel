#ifndef Standing_H
#define	Standing_H

#include <string>
#include <Behaviors/Behavior.h>
#include <Core/Human.h>

using namespace CartWheel::Behaviors;

namespace CartWheel {
    class CartWheel3D;
    namespace Behaviors {

        class Standing : public Behavior {  
        private:            
            Core::Human* human;
            Core::BehaviourController* bcontroller;
            
        protected:
            virtual void runStep();
            virtual void onInit();
            virtual void onFinish();
            
        public:
            Standing(CartWheel3D* cw, std::string humanName, Standing_Params* params);
        };
    }
}

#endif

