#include "Behaviors/WalkInArc.h"
#include <Core/CartWheel3D.h>

using namespace CartWheel;
using namespace CartWheel::Core;

WalkInArc::WalkInArc(CartWheel3D* cw, std::string humanName, WalkInArc_Params* params)
        : Behavior(cw, humanName, params!=NULL ? params->startTime : 0, params!=NULL ? params->duration : 0) {
    nSpeed = params->speed;
    nAngSpeed = params->angSpeed;
    //    nStep = (nAngSpeed/abs(nAngSpeed))*0.5;

    this->cw->getHuman(humanName, &human);
    bcontroller = human->getBehaviour();
}

void WalkInArc::onInit() {
    cw->setHumanSpeed(humanName, nSpeed);
}

void WalkInArc::runStep() {
    nSteps++;
    if (nSteps == SimGlobals::steps_per_second / 2) {
        nSteps = 0;
        cw->setHumanHeading(humanName, human->getHeading() + nAngSpeed / 2);
        printf("Time: %f / %f, ANGLE: %f\n", time + 1, duration, human->getHeading());
    }
}

void WalkInArc::onFinish() {
}

