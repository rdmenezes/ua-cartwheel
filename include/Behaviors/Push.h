/* 
 * File:   Push.h
 * Author: alfredo
 *
 * Created on May 19, 2011, 3:09 PM
 */

#ifndef PUSH_H
#define	PUSH_H

#include <string>
#include <Behaviors/Behavior.h>

using namespace CartWheel::Behaviors;

namespace CartWheel {
    class CartWheel3D;
    namespace Behaviors {

        class Push : public Behavior {            
        protected:
            virtual void runStep();
            virtual void onInit();
            virtual void onFinish();
            
        public:
            Push(CartWheel3D* cw, std::string humanName, Push_Params* params);
        };
    }
}

#endif	/* PUSH_H */

