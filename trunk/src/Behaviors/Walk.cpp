/* 
 * File:   Standing.cpp
 * Author: alfredo
 * 
 * Created on May 19, 2011, 2:56 PM
 */

#include "Behaviors/Walk.h"
#include <Core/CartWheel3D.h>

using namespace CartWheel;

#ifndef isnan
#define isnan(x) (x != x)
#define MUST_UNDEF_ISNAN
#endif

Walk::Walk(CartWheel3D* cw, std::string humanName, Walk_Params* params)
        : Behavior(cw, humanName, params!=NULL ? params->startTime : 0, params!=NULL ? params->duration : 0) {
    nSpeed = params->speed;
    nAngle = params->angle;
    printf("StartTime %f, Duration %f, Speed %f, Angle %f.\n", startTime, duration, nSpeed, nAngle);
}

void Walk::onInit() {
    if (!isnan(nSpeed)) {
        printf("Init Walking Time: %f\n", time);
        printf("Starting to walk... (speed=%f)\n", nSpeed);
        cw->setHumanSpeed(humanName, nSpeed);
    }
    if (!isnan(nAngle)) {
        printf("Starting to turn... (angle=%f)\n", nAngle);
        cw->setHumanHeading(humanName, nAngle);
    }
}

void Walk::runStep() {
}

void Walk::onFinish() {
    cw->setHumanSpeed(humanName, 0);
}

#ifdef MUST_UNDEF_ISNAN
#undef isnan(x) (x != x)
#undef MUST_UNDEF_ISNAN
#endif
