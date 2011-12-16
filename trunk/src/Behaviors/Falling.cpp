#include "Behaviors/Falling.h"
#include <Core/CartWheel3D.h>

using namespace CartWheel;

Falling::Falling(CartWheel3D* cw, std::string humanName, Falling_Params* params)
        : Behavior(cw, humanName, params!=NULL ? params->startTime : 0, params!=NULL ? params->duration : 0) {
    this->cw->getHuman(humanName, &human);
    bcontroller = human->getBehaviour();
    if(params == NULL) {
        printf("Params are NULL!!!\n\n");
    } else {
        printf("Params are %f, %f!!!\n\n", params->duration, params->startTime);
    }
}

void Falling::onInit() {
//    cw->setHumanSpeed(humanName, 0);
//    bcontroller->requestStepHeight(-0.2);
//    bcontroller->requestStepTime(100);
    cw->setController(humanName, 1);
}

void Falling::runStep() {
}  


void Falling::onFinish() {
//    bcontroller->requestStepHeight(0.1);
//    bcontroller->requestStepTime(0.5);
}

