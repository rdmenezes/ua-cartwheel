#ifndef Walk_H
#define	Walk_H

#include <string>
#include <Behaviors/Behavior.h>
#include <Core/BehaviourController.h>
#include <Core/Human.h>

using namespace CartWheel::Behaviors;

namespace CartWheel {
    class CartWheel3D;
    namespace Behaviors {

        class Walk : public Behavior {
        private:
            double nSpeed;
            double nAngle;
            Core::Human* human;
            Core::BehaviourController* bcontroller;
            
        protected:
            virtual void runStep();
            virtual void onInit();
            virtual void onFinish();

        public:
            Walk(CartWheel3D* cw, std::string humanName, Walk_Params* params);
        };
    }
}

#endif

