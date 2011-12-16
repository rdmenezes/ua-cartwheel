#include "Behaviors/Push.h"
#include <Core/CartWheel3D.h>

using namespace CartWheel;

SitDown::SitDown(CartWheel3D* cw, std::string humanName, SitDown_Params* params)
        : Behavior(cw, humanName, params!=NULL ? params->startTime : 0, params!=NULL ? params->duration : 0) {
    this->sTargetObj = params->sTargetObj;
    this->cw->getHuman(humanName, &human);
    bcontroller = human->getBehaviour();
    nDuration = params->duration;
}

void SitDown::onInit() {
//    cw->setController(humanName, 5); //1
    bcontroller->requestStepHeight(-0.08);
    nFrame = 0;
} 

void SitDown::runStep() {    
    double nan = std::numeric_limits<double>::quiet_NaN();
    int nF = 0;
    Quaternion qLowerBack, qTorso;
    
    nSteps++;
    if (nSteps == SimGlobals::steps_per_second/2) {
        nSteps = 0;
        nF = nFrame;
        
        if (nFrame==0) {
            //Twist, Sagital (Up-Down), Coronal (Sides)
//            bcontroller->requestShoulderBend(1.5,1.5, -1.3,1.3, -1.5,1.5);
//            bcontroller->requestElbowBend()
            bcontroller->requestStepHeight(-0.08);
            bcontroller->requestStepTime(5);
//            bcontroller->requestUpperBodyPose(10,0,0);
            bcontroller->requestElbowBend(0,0, -0.25,0.25, 0,0);
//            bcontroller->requestKneeBend(1);
//            bcontroller->requestUpperBodyPose(0.2,0,0);
//            bcontroller->requestHandBend(0,0, 0,0, 0,0);
        }
        if (nFrame==1) {
//            bcontroller->requestStepHeight(-0.01);
            human->setVelocity("head", Vector3d(0,0,5));
            human->setVelocity("lowerBack", Vector3d(0,0,-5));
            human->setVelocity("pelvis", Vector3d(0,0,-2));
            bcontroller->requestElbowBend(0,0, -0.75,0.75, 0,0);
            bcontroller->requestKneeBend(1.2);
        }
        if (nFrame==2) {
            qLowerBack = human->getOrientationQ("lowerBack");
            qTorso = human->getOrientationQ("torso");
            bcontroller->requestKneeBend(1.55);
////            human->setVelocity("head", Vector3d(0,0,8));
//            human->setVelocity("torso", Vector3d(0,0,3));
//            human->setVelocity("lowerBack", Vector3d(0,0,-6));
//            human->setVelocity("pelvis", Vector3d(0,0,-1));
        }
        if (nFrame>=2 && nFrame<9) {
            human->factorVelocity("lowerBack", Vector3d(0,1,0));
            human->factorVelocity("torso", Vector3d(0,1,0));
            human->factorVelocity("head", Vector3d(0,1,0));
            human->factorVelocity("pelvis", Vector3d(0,1,0));
//            human->factorAngVelocity("lowerBack", Vector3d(0,0,0));
//            human->factorAngVelocity("torso", Vector3d(0,0,0));
//            human->factorAngVelocity("head", Vector3d(0,0,0));
//            human->factorAngVelocity("pelvis", Vector3d(0,0,0));
//            human->setOrientationQ("lowerBack", qLowerBack);
//            human->setOrientationQ("torso", qTorso);
        }
//        if (nFrame==9) {            
//            bcontroller->requestStepTime(0.6);
//            bcontroller->requestStepHeight(0.1);
//        }
        if (nFrame==8) {
            human->setVelocity("head", Vector3d(0,0,7));
            human->setVelocity("lowerBack", Vector3d(0,0,1.5));
            human->setVelocity("pelvis", Vector3d(0,0,2.5));
            human->setVelocity("lHand", Vector3d(0,0,7));
            human->setVelocity("rHand", Vector3d(0,0,7));
            bcontroller->requestElbowBend(0,0, 0,0, 0,0);
            bcontroller->requestStepTime(0.8);
            bcontroller->requestStepHeight(-0.01);
            bcontroller->requestKneeBend(0.5);
            cw->setController(humanName, 0);
            cw->setHumanSpeed(humanName, 0.6);
        }
//        if (nFrame==9) {
//            bcontroller->requestKneeBend(0);
//        }
        nFrame++;
    }
}

void SitDown::onFinish() {
//    human->setVelocity("head", Vector3d(0,0,120));
//    human->setVelocity("torso", Vector3d(0,0,8000));
//    human->setVelocity("lowerBack", Vector3d(0,0,8000));
//    human->setVelocity("pelvis", Vector3d(0,6,10000));
//    bcontroller->requestShoulderBend(0,0, 0,0, -1.5,1.5);
//    bcontroller->requestElbowBend(0,0, 0,0, 0,0);
//    bcontroller->requestStepTime(0.6);
//    bcontroller->requestStepHeight(0.1);
//    bcontroller->requestKneeBend(0);
//    human->setOrientationQ("lElbow", Quaternion(0,0,0,0));
//    human->setOrientationQ("rElbow", Quaternion(0,0,0,0));
    bcontroller->requestKneeBend(0);
    cw->setHumanSpeed(humanName, 0);
//    bcontroller->requestUpperBodyPose(0,0,0);
//    bcontroller->requestHandBend(0,0, 0,0, 0,0);
//    bcontroller->requestStepTime(0.8);
//    bcontroller->requestStepHeight(-0.05);
}
