/* 
 * File:   WaveHand.cpp
 * Author: alfredo
 * 
 * Created on Jun 26, 2011, 2:56 PM
 */

#include "Behaviors/WaveHand.h"
#include <Core/CartWheel3D.h>

using namespace CartWheel;

WaveHand::WaveHand(CartWheel3D* cw, std::string humanName, WaveHand_Params* params)
        : Behavior(cw, humanName, params!=NULL ? params->startTime : 0, params!=NULL ? params->duration : 0) {
    sHand = params->sHand.c_str();
}

void WaveHand::onInit() {
    printf("Waving hand: %s!!\n", sHand.c_str());
    cw->setController(humanName, 10);
}

void WaveHand::runStep() {
}

void WaveHand::onFinish() {
    cw->setController(humanName, 0);
    printf("Ending WavingHand Time: %f\n", time);
}

