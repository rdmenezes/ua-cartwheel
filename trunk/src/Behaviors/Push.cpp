/*
 * File:   Push.cpp
 * Author: alfredo
 *
 * Created on May 19, 2011, 2:56 PM
 */

#include "Behaviors/Push.h"
#include <Core/CartWheel3D.h>

using namespace CartWheel;

Push::Push(CartWheel3D* cw, std::string humanName, Push_Params* params)
        : Behavior(cw, humanName, params!=NULL ? params->startTime : 0, params!=NULL ? params->duration : 0) {
}

void Push::onInit() {
    cw->setController(humanName, 5); //1
} 

void Push::runStep() {
}

void Push::onFinish() {
}
