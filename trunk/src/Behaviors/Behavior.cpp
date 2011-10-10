/* 
 * File:   Behavior.cpp
 * Author: alfredo
 * 
 * Created on June 29, 2011, 2:32 AM
 */

#include "Behaviors/Behavior.h"
#include <Core/CartWheel3D.h>

using namespace CartWheel;
using namespace CartWheel::Core;

Behavior::Behavior() {
}

Behavior::Behavior(CartWheel3D* cw, std::string humanName, double startTime, double duration) {
    this->cw = cw;
    this->humanName = humanName;
    this->duration = duration;
    this->startTime = startTime;
    this->endTime = startTime + duration;
    this->steps_per_second = SimGlobals::steps_per_second;
    this->dt = SimGlobals::dt;
    this->time = 0;
    this->nSteps = 0;
}

Behavior::~Behavior() {
}

bool Behavior::runStep(double time) {
    this->time = time;    
    bool isInit = (time >= startTime) && ((time - dt < startTime) || (time - dt == 0));
    bool isFinish = (time < endTime) && (time + dt >= endTime);
    bool isWaiting = (time < startTime);
    bool isRunning = (time >= startTime) && (time < endTime);
    
    if(isInit) {
//        printf("init\n");
        onInit();
    } if(isFinish) {
//        printf("finish\n");
        onFinish();
    } if(isRunning) {
//        printf("running\n");
        runStep();
    }
    
    return (isRunning || isWaiting);
}

