#include "Behaviors/Push.h"
#include <Core/CartWheel3D.h>

using namespace CartWheel;

Jump::Jump(CartWheel3D* cw, std::string humanName, Jump_Params* params)
        : Behavior(cw, humanName, params!=NULL ? params->startTime : 0, params!=NULL ? params->duration : 0) {
    this->sDirection = params->sDirection;
    this->cw->getHuman(humanName, &human);
    bcontroller = human->getBehaviour();
    nDuration = params->duration;
    strength = params->strength;
}

void Jump::onInit() {
    nFrame = 0;
} 

void Jump::runStep() {    
    double nan = std::numeric_limits<double>::quiet_NaN();
    int nF = 0;
    
    nSteps++;
    if (nSteps == SimGlobals::steps_per_second/4) {
        nSteps = 0;
        nF = nFrame;
        
        if (nFrame==0) {
            bcontroller->requestShoulderBend(0,0, 1.8,-1.8, -1.4,1.4);
            bcontroller->requestPelvisTorsoBend(0.5,0,0, 0.5,0,0);
            bcontroller->requestKneeBend(1.2);            
        }
        if (nFrame==1) {            
            bcontroller->requestShoulderBend(0,0, 0.5,-0.5, -1.2,1.2);
            bcontroller->requestElbowBend(0,0, -0.8,0.8, 0,0);
            bcontroller->requestPelvisTorsoBend(2,0,0, 0.2,0,0);
        }
        if (nFrame==2) {
            bcontroller->requestShoulderBend(0,0, -0.4,0.4, -0.8,0.8);
//            human->setVelocity("torso", Vector3d(0,12,1));
//            human->setVelocity("pelvis", Vector3d(0,12,9));
//            bcontroller->requestKneeBend(0.3);
            
//            //Jump Rotating Backwards
//            human->setVelocity("head", Vector3d(0,7.3,-7.7));
//            human->setVelocity("torso", Vector3d(0,7.3,-7.7));
//            human->setVelocity("pelvis", Vector3d(0,20,20));
//            bcontroller->requestKneeBend(0.3);
            
            //Jump Rotating Forward
            human->setVelocity("head", Vector3d(0,7,7));
            human->setVelocity("torso", Vector3d(0,13,12));
            human->setVelocity("pelvis", Vector3d(0,12,-10));
            bcontroller->requestKneeBend(0.3);
        }
        if (nFrame>=4) {
            bcontroller->requestShoulderBend(0,0, 0,0, -1.4,1.4);
            bcontroller->requestElbowBend(0,0, 0,0, 0,0);
            bcontroller->requestPelvisTorsoBend(0,0,0, 0,0,0);
            human->setLockPosition("torso", true, true, false);
            human->setLockPosition("lowerBack", true, true, false);
            human->setLockPosition("pelvis", true, true, false);
            human->setLockOrientation("torso", true, true, true);
            human->setLockOrientation("lowerBack", true, true, true);
            human->setLockOrientation("pelvis", true, true, true);
        }
        nFrame++;
    }
}

void Jump::onFinish() {
    bcontroller->requestKneeBend(0);
    cw->setController(humanName, 0);
}
