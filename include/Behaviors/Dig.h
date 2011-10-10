/* 
 * File:   Dig.h
 * Author: alfredo
 *
 * Created on June 8, 2011, 10:24 AM
 */

#ifndef DIG_H
#define	DIG_H

#include <string>
#include <Behaviors/Behavior.h>
#include <Core/Human.h>

using namespace CartWheel::Behaviors;

namespace CartWheel {
    class CartWheel3D;
    namespace Behaviors {

        class Dig : public Behavior {
        private:            
            Core::Human* human;
            bool bDigFinishing;
            Quaternion qPelvis, qlUpperLeg, qlLowerLeg;
            Quaternion qrUpperLeg, qrLowerLeg;
            
        protected:
            virtual void runStep();
            virtual void onInit();
            virtual void onFinish();
            
        public:
            Dig(CartWheel3D* cw, std::string humanName, Dig_Params* params);
        };
    }
}

#endif	/* DIG_H */

