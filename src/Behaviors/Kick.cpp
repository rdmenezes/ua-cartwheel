#include "Behaviors/Kick.h"
#include "Control/ActRecognizerInterface.h"
#include <Core/CartWheel3D.h>

using namespace CartWheel;

Kick::Kick(CartWheel3D* cw, std::string humanName, Kick_Params* params)
        : Behavior(cw, humanName, params!=NULL ? params->startTime : 0, params!=NULL ? params->duration : 0) {
    this->cw->getHuman(humanName, &human);
    bKickFinishing = false;
    bKicking = false;
}

void Kick::onInit() {
    printf("starting to kick\n");    
//    if (human->getFootTouchedGround()==true)
//        printf("Left Foot Touching Ground!!!\n");
//        cw->setController(humanName, 9);
//        bKicking = true;
//        qUpperLeg = human->getOrientationQ("lUpperLeg");
//        qLowerLeg = human->getOrientationQ("lLowerLeg");
    nSteps = 0;
}

void Kick::runStep() {    
    if (time > endTime-duration*0.5 and bKickFinishing==false) {
        cw->setController(humanName, 0);
        printf("-------Time: %f", time);
        bKickFinishing = true;
    }
//    if (human->getFootTouchedGround()==true)
//        printf("Foot on Ground!!!, %f\n", time);
//    else
//        printf("----\n");
    if (human->getFootTouchedGround()==true and bKicking==false) {
//        printf("Foot on Ground!!!, %d\n", nSteps);
        nSteps++;
    }
    if (nSteps>30 and bKicking==false) {
        cw->setController(humanName, 9);
        qUpperLeg = human->getOrientationQ("lUpperLeg");
        qLowerLeg = human->getOrientationQ("lLowerLeg");
        bKicking = true;
    } else if(nSteps>30) {        
        human->setLock("pelvis", true, true);
        human->setLock("torso", true, true);
        human->setLock("lUpperLeg", true, true);
        human->setLock("lLowerLeg", true, true);
        human->setOrientationQ("lUpperLeg", qUpperLeg);
        human->setOrientationQ("lLowerLeg", qLowerLeg);
    }
}

void Kick::onFinish() {
    cw->setController(humanName, 0);
//    printf("finished kicking\n");
}
