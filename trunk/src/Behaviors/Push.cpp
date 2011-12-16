#include "Behaviors/Push.h"
#include <Core/CartWheel3D.h>

using namespace CartWheel;

Push::Push(CartWheel3D* cw, std::string humanName, Push_Params* params)
        : Behavior(cw, humanName, params!=NULL ? params->startTime : 0, params!=NULL ? params->duration : 0) {
    this->cw->getHuman(humanName, &human);
    bcontroller = human->getBehaviour();
}

void Push::onInit() {
//    cw->setController(humanName, 5); //1
    nFrame = 0;
} 

void Push::runStep() {    
    double nan = std::numeric_limits<double>::quiet_NaN();
    int nF = 0;
    
    nSteps++;
    if (nSteps == SimGlobals::steps_per_second/2) {
        nSteps = 0;
        nF = nFrame;        
        
//        human->setLockOrientation("lLowerArm", true, true, true);
//        human->setLockOrientation("rLowerArm", true, true, true);
//        human->getCharacter()->getJointByName("lElbow")->
        if (nFrame==0) {
            //Twist, Sagital (Up-Down), Coronal (Sides)
            bcontroller->requestShoulderBend(1.4,1.4, -1.8,1.8, -1.5,1.5);
            bcontroller->requestKneeBend(0.1);
            bcontroller->requestHandBend(0,0, 0,0, 0,0);
        }
        if (nFrame==1) {
            bcontroller->requestHandBend(0,0, 0,0, 1.5,-1.5);
            bcontroller->requestPelvisTorsoBend(0.8,0,0, -0.2,0,0);
        }
        nFrame++;
    }
}

void Push::onFinish() {
    bcontroller->requestShoulderBend(0,0, 0,0, -1.5,1.5);
    bcontroller->requestKneeBend(0);
    bcontroller->requestPelvisTorsoBend(0,0,0, 0,0,0);
    bcontroller->requestHandBend(0,0, 0,0, 0,0);
}
