/* 
 * File:   Behavior.h
 * Author: alfredo
 *
 * Created on May 18, 2011, 1:25 PM
 */

#ifndef BEHAVIOR_H
#define	BEHAVIOR_H

#include <string>
#include <Behaviors/Params.h>
#include <Core/SimGlobals.h>


namespace CartWheel {
    class CartWheel3D; 
    namespace Behaviors {

        class Behavior {
        protected:
            double time;
            double steps_per_second;
            double dt;
            double duration;
            double startTime;
            double endTime;
            int nSteps;
            CartWheel3D* cw;
            std::string humanName;


        protected:
            virtual void runStep() = 0;
            virtual void onInit() = 0;
            virtual void onFinish() = 0;


        public:
            Behavior();
            Behavior(CartWheel3D* cw, std::string humanName, double startTime, double duration);
            bool runStep(double time);
            virtual ~Behavior();
        };
    }
}

#endif	/* BEHAVIOR_H */

