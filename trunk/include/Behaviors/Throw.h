#ifndef Throw_H
#define	Throw_H

#include <string>
#include <Behaviors/Behavior.h>
#include <Core/BehaviourController.h>
#include <Core/Human.h>

using namespace CartWheel::Behaviors;

namespace CartWheel {
    class CartWheel3D;
    namespace Behaviors {
        
        class Throw : public Behavior {
        private:
            int nFrame;
            std::string sHand, sObj;
            Vector3d speed;
            Core::Human* human;
            Core::BehaviourController* bcontroller;
            
        protected:
            virtual void runStep();
            virtual void onInit();
            virtual void onFinish();

        public:
            Throw(CartWheel3D* cw, std::string humanName, Throw_Params* params);
        };
    }
}

#endif

