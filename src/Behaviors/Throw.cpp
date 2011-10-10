/* 
 * File:   Throw.cpp
 * Author: alfredo
 * 
 * Created on Jun 26, 2011, 2:56 PM
 */

#include "Behaviors/Throw.h"
#include <Core/CartWheel3D.h>

using namespace CartWheel;

Throw::Throw(CartWheel3D* cw, std::string humanName, Throw_Params* params)
        : Behavior(cw, humanName, params!=NULL ? params->startTime : 0, params!=NULL ? params->duration : 0) {
    sHand = params->sHand.c_str();
    printf("1Waving hand: %s!!\n", sHand.c_str());
    this->cw->getHuman(humanName, &human);
    printf("2Waving hand: %s!!\n", sHand.c_str());
    bcontroller = human->getBehaviour();
    printf("3Waving hand: %s!!\n", sHand.c_str());
}

void Throw::onInit() {
    printf("Waving hand: %s!!\n", sHand.c_str());
//    if(strcmp(sHand.c_str(), "Left")==0)
//        cw->setController(humanName, 10);
//    else if(strcmp(sHand.c_str(), "Right")==0)
//        cw->setController(humanName, 11);
//    else if(strcmp(sHand.c_str(), "Both")==0)
//        cw->setController(humanName, 12);
    human->getController()->setStance(RIGHT_STANCE);
    cw->setController(humanName, 15);
//    human->getCharacter()->getARBByName("lUpperLeg")->getOrientation()
    nFrame = 0;
}

void Throw::runStep() {
    nSteps++;
    if (nSteps == SimGlobals::steps_per_second/2) {
        nSteps = 0;
//        human->factorVelocity("pelvis", Vector3d(0.8,0.8,0.8));
        nFrame++;
        if (nFrame==3) {
            printf("Throwing Fire!!!\n");
            cw->makeHumanThrowObject("Human1", "ball1", Vector3d(1,1,3));
        }        
    }
}

void Throw::onFinish() {
    double nan = std::numeric_limits<double>::quiet_NaN();
    //Left    
    if(strcmp(sHand.c_str(), "Left")==0 || strcmp(sHand.c_str(), "Both")==0) {
        printf("---Finish Left\n");
        bcontroller->requestShoulderBend(0, nan, 0, nan, -1.5, nan);
        bcontroller->requestElbowBend(0, nan, 0, nan, 0, nan);
    }
    //Right
    if(strcmp(sHand.c_str(), "Right")==0 || strcmp(sHand.c_str(), "Both")==0) {
        printf("---Finish Right\n");
        bcontroller->requestShoulderBend(nan, 0, nan, 0, nan, 1.5);
        bcontroller->requestElbowBend(nan, 0, nan, nan, nan, 0);
    }
    human->getController()->setStance(LEFT_STANCE);
    cw->setController(humanName, 0);
    printf("Ending WavingHand Time: %f\n", time);
}

