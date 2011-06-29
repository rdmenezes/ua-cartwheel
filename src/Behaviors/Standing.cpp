/* 
 * File:   Standing.cpp
 * Author: alfredo
 * 
 * Created on May 19, 2011, 2:56 PM
 */

#include "Behaviors/Standing.h"
#include <Core/CartWheel3D.h>

using namespace CartWheel;

Standing::Standing(CartWheel3D* cw, std::string humanName, Standing_Params* params)
        : Behavior(cw, humanName, params!=NULL ? params->startTime : 0, params!=NULL ? params->duration : 0) {
    if(params == NULL) {
        printf("Params are NULL!!!\n\n");
    } else {
        printf("Params are %f, %f!!!\n\n", params->duration, params->startTime);
    }
}

void Standing::onInit() {    
    cw->setController(humanName, 0);
}  

void Standing::runStep() {
}  


void Standing::onFinish() {    
}

