/* 
 * File:   Kick.h
 * Author: alfredo
 *
 * Created on June 8, 2011, 10:24 AM
 */

#ifndef KICK_H
#define	KICK_H

#include <string>
#include <Behaviors/Behavior.h>
#include <Core/Human.h>

using namespace CartWheel::Behaviors;

namespace CartWheel {
    class CartWheel3D;
    namespace Behaviors {

        class Kick : public Behavior {
        private:            
            Core::Human* human;
            bool bKickFinishing, bKicking;
            Quaternion qUpperLeg, qLowerLeg;
            
        protected:
            virtual void runStep();
            virtual void onInit();
            virtual void onFinish();
            
        public:
            Kick(CartWheel3D* cw, std::string humanName, Kick_Params* params);
        };
    }
}

#endif	/* KICK_H */

