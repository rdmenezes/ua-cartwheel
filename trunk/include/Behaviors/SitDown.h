#ifndef SitDown_H
#define	SitDown_H

#include <string>
#include <Behaviors/Behavior.h>
#include <Core/BehaviourController.h>
#include <Core/Human.h>

using namespace CartWheel::Behaviors;

namespace CartWheel {
    class CartWheel3D;
    namespace Behaviors {

        class SitDown : public Behavior {
        private:
            int nFrame;
            double nDuration;
            std::string sTargetObj;
            Core::Human* human;
            Core::BehaviourController* bcontroller;
            
        protected:
            virtual void runStep();
            virtual void onInit();
            virtual void onFinish();
            
        public:
            SitDown(CartWheel3D* cw, std::string humanName, SitDown_Params* params);
        };
    }
}

#endif

