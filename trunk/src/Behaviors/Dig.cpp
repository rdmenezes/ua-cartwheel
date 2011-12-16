#include "Behaviors/Dig.h"
#include "Control/ActRecognizerInterface.h"
#include <Core/CartWheel3D.h>

using namespace CartWheel;

Dig::Dig(CartWheel3D* cw, std::string humanName, Dig_Params* params)
        : Behavior(cw, humanName, params!=NULL ? params->startTime : 0, params!=NULL ? params->duration : 0) {
    this->cw->getHuman(humanName, &human);
    bDigFinishing = false;
}

void Dig::onInit() {
    printf("starting to dig\n");
    cw->setController(humanName, 13);
    human->getController()->setStance(RIGHT_STANCE);
    qPelvis = human->getOrientationQ("pelvis");
    qlUpperLeg = human->getOrientationQ("lUpperLeg");
    qlLowerLeg = human->getOrientationQ("lLowerLeg");
    qrUpperLeg = human->getOrientationQ("rUpperLeg");
    qrLowerLeg = human->getOrientationQ("rLowerLeg");
    
    human->setLock("pelvis", true, true);
    human->setLock("lowerBack", true, false);
    human->setLock("lUpperLeg", true, true);
    human->setLock("lLowerLeg", true, true);
    human->setLock("rLowerLeg", true, true);
    human->setLock("rUpperLeg", true, true);
    human->setOrientationQ("pelvis", qPelvis);
    human->setOrientationQ("lUpperLeg", qlUpperLeg);
    human->setOrientationQ("lLowerLeg", qlLowerLeg);
    human->setOrientationQ("rUpperLeg", qrUpperLeg);
    human->setOrientationQ("rLowerLeg", qrLowerLeg);
//    human->setLock("lUpperLeg", true, true);
//    human->setLock("lLowerLeg", true, true);
//    human->setLock("rLowerLeg", true, true);
//    human->setLock("rUpperLeg", true, true);
}

void Dig::runStep() {    
//    if (time > endTime-duration*0.9 and bDigFinishing==false) {
//        cw->setController(humanName, 0);
//        printf("-------Time: %f", time);
//        bDigFinishing = true;
//    }
    human->setLock("pelvis", true, true);
    human->setLock("lowerBack", true, false);
//    human->setLock("torso", true, true);
    human->setLock("lUpperLeg", true, true);
    human->setLock("lLowerLeg", true, true);
    human->setLock("rLowerLeg", true, true);
    human->setLock("rUpperLeg", true, true);
    human->setOrientationQ("pelvis", qPelvis);
    human->setOrientationQ("lUpperLeg", qlUpperLeg);
    human->setOrientationQ("lLowerLeg", qlLowerLeg);
    human->setOrientationQ("rUpperLeg", qrUpperLeg);
    human->setOrientationQ("rLowerLeg", qrLowerLeg);
}

void Dig::onFinish() {
    cw->setController(humanName, 0);
    human->getController()->setStance(LEFT_STANCE);
}
