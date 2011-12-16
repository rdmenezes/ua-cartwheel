#include "Behaviors/Standing.h"
#include <Core/CartWheel3D.h>

using namespace CartWheel;

Standing::Standing(CartWheel3D* cw, std::string humanName, Standing_Params* params)
        : Behavior(cw, humanName, params!=NULL ? params->startTime : 0, params!=NULL ? params->duration : 0) {
    this->cw->getHuman(humanName, &human);
    bcontroller = human->getBehaviour();
    if(params == NULL) {
        printf("Params are NULL!!!\n\n");
    } else {
        printf("Params are %f, %f!!!\n\n", params->duration, params->startTime);
    }
}

void Standing::onInit() {
    cw->setHumanSpeed(humanName, 0);
//    cw->setController(humanName, 1);
    bcontroller->requestStepHeight(-0.2);
    bcontroller->requestStepTime(2);
}  

void Standing::runStep() {
//    double dF = 0.7;
//    human->factorVelocity("pelvis", Vector3d(dF,dF,dF));
//    human->factorVelocity("torso", Vector3d(dF,dF,dF));
//    human->factorVelocity("head", Vector3d(dF,dF,dF));
    
    human->setLock("torso", true, true);
    human->setLock("pelvis", true, true);
}  


void Standing::onFinish() {
//    cw->setController(humanName, 0);
    bcontroller->requestStepHeight(0.1);
    bcontroller->requestStepTime(0.5);
}

