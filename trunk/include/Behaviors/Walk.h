/* 
 * File:   Standing.h
 * Author: alfredo
 *
 * Created on May 19, 2011, 2:42 PM
 */

#ifndef WALK_H
#define	WALK_H

#include <string>
#include <Behaviors/Behavior.h>

using namespace CartWheel::Behaviors;

namespace CartWheel {
    class CartWheel3D;
    namespace Behaviors {

        class Walk : public Behavior {
        private:
            double nSpeed;
            double nAngle;
            
        protected:
            virtual void runStep();
            virtual void onInit();
            virtual void onFinish();

        public:
            Walk(CartWheel3D* cw, std::string humanName, Walk_Params* params);
        };
    }
}

#endif	/* WALK_H */

