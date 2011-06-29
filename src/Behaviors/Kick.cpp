/* 
 * File:   Kick.cpp
 * Author: alfredo
 * 
 * Created on June 8, 2011, 10:24 AM
 */

#include "Behaviors/Kick.h"
#include "Control/ActRecognizerInterface.h"
#include <Core/CartWheel3D.h>

using namespace CartWheel;

Kick::Kick(CartWheel3D* cw, std::string humanName, Kick_Params* params)
        : Behavior(cw, humanName, params!=NULL ? params->startTime : 0, params!=NULL ? params->duration : 0) {
}

void Kick::onInit() {
    cw->setController(humanName, 9);
}

void Kick::runStep() {
//    nTime++;
//    if(nTime == 1100) {
//        cw->doBehavior("Standing", sHumanName, NULL);
//    }
}

void Kick::onFinish() {
}
