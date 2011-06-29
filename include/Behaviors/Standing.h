/* 
 * File:   Standing.h
 * Author: alfredo
 *
 * Created on May 19, 2011, 2:42 PM
 */

#ifndef STANDING_H
#define	STANDING_H

#include <string>
#include <Behaviors/Behavior.h>

using namespace CartWheel::Behaviors;

namespace CartWheel {
    class CartWheel3D;
    namespace Behaviors {

        class Standing : public Behavior {            
        protected:
            virtual void runStep();
            virtual void onInit();
            virtual void onFinish();
            
        public:
            Standing(CartWheel3D* cw, std::string humanName, Standing_Params* params);
        };
    }
}

#endif	/* STANDING_H */

