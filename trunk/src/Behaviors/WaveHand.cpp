#include "Behaviors/WaveHand.h"
#include <Core/CartWheel3D.h>

using namespace CartWheel;

WaveHand::WaveHand(CartWheel3D* cw, std::string humanName, WaveHand_Params* params)
        : Behavior(cw, humanName, params!=NULL ? params->startTime : 0, params!=NULL ? params->duration : 0) {
    sHand = params->sHand.c_str();
    printf("1Waving hand: %s!!\n", sHand.c_str());
    this->cw->getHuman(humanName, &human);
    printf("2Waving hand: %s!!\n", sHand.c_str());
    bcontroller = human->getBehaviour();
    printf("3Waving hand: %s!!\n", sHand.c_str());
}

void WaveHand::onInit() {
    printf("Waving hand: %s!!\n", sHand.c_str());
//    if(strcmp(sHand.c_str(), "Left")==0)
//        cw->setController(humanName, 10);
//    else if(strcmp(sHand.c_str(), "Right")==0)
//        cw->setController(humanName, 11);
//    else if(strcmp(sHand.c_str(), "Both")==0)
//        cw->setController(humanName, 12);
//    cw->setController(humanName, 15);
//    human->getCharacter()->getARBByName("lUpperLeg")->getOrientation()
    nFrame = 0;
}

void WaveHand::runStep() {    
//    human->setLock("pelvis", true, true);
//    human->setLock("lowerBack", true, true);
//    human->setLock("torso", true, true);
////    human->setLock("head", true, true);
////    human->setLock("lUpperArm", true, true);
////    human->setLock("rUpperArm", true, true);
////    human->setLock("lLowerArm", true, true);
////    human->setLock("rLowerArm", true, true);
////    human->setLock("lHand", true, true);
////    human->setLock("rHand", true, true);
//    human->setLock("lUpperLeg", true, true);
//    human->setLock("lLowerLeg", true, true);
//    human->setLock("rUpperLeg", true, true);
//    human->setLock("rLowerLeg", true, true);
//    human->setLock("lFoot", true, true);
//    human->setLock("rFoot", true, true);
//    human->setLock("lToes", true, true);
//    human->setLock("rToes", true, true);
//    
//    human->setOrientation("pelvis", Vector3d(-1.5,0,0), false);
//    human->setOrientation("lowerBack", Vector3d(-1.5,0,0), false);
//    human->setOrientation("torso", Vector3d(-1.5,0,0), false);
//    
//    human->setOrientation("lUpperLeg", Vector3d(0,0,0), false);
//    human->setOrientation("lLowerLeg", Vector3d(0,0,0), false);
//    human->setOrientation("rUpperLeg", Vector3d(0,0,0), false);
//    human->setOrientation("rLowerLeg", Vector3d(0,0,0), false);
//    human->setOrientation("lFoot", Vector3d(0,0,0), false);
//    human->setOrientation("rFoot", Vector3d(0,0,0), false);
//    human->setOrientation("lToes", Vector3d(0,0,0), false);
//    human->setOrientation("rToes", Vector3d(0,0,0), false);
////    printf("Panic %f\n", human->getBehaviour()->getPanicLevel());
    
////    human->setExternalForce("pelvis", Vector3d(0,0,0), false);
////    human->setExternalForce("lowerBack", Vector3d(0,0,0), false);
////    human->setExternalForce("torso", Vector3d(0,0,0), false);
////    human->setExternalForce("head", Vector3d(0,0,0), false);
////    human->setExternalForce("lUpperArm", Vector3d(0,0,0), false);
////    human->setExternalForce("rUpperArm", Vector3d(0,0,0), false);
////    human->setExternalForce("lLowerArm", Vector3d(0,0,0), false);
////    human->setExternalForce("rLowerArm", Vector3d(0,0,0), false);
////    human->setExternalForce("lHand", Vector3d(0,0,0), false);
////    human->setExternalForce("rHand", Vector3d(0,0,0), false);
////    human->setExternalForce("lUpperLeg", Vector3d(0,0,0), false);
////    human->setExternalForce("lLowerLeg", Vector3d(0,0,0), false);
////    human->setExternalForce("rUpperLeg", Vector3d(0,0,0), false);
////    human->setExternalForce("rLowerLeg", Vector3d(0,0,0), false);
////    human->setExternalForce("lFoot", Vector3d(0,0,0), false);
////    human->setExternalForce("rFoot", Vector3d(0,0,0), false);
////    human->setExternalForce("lToes", Vector3d(0,0,0), false);
////    human->setExternalForce("rToes", Vector3d(0,0,0), false);
////    
////    human->setExternalTorque("pelvis", Vector3d(0,0,0), false);
////    human->setExternalTorque("lowerBack", Vector3d(0,0,0), false);
////    human->setExternalTorque("torso", Vector3d(0,0,0), false);
////    human->setExternalTorque("head", Vector3d(0,0,0), false);
////    human->setExternalTorque("lUpperArm", Vector3d(0,0,0), false);
////    human->setExternalTorque("rUpperArm", Vector3d(0,0,0), false);
////    human->setExternalTorque("lLowerArm", Vector3d(0,0,0), false);
////    human->setExternalTorque("rLowerArm", Vector3d(0,0,0), false);
////    human->setExternalTorque("lHand", Vector3d(0,0,0), false);
////    human->setExternalTorque("rHand", Vector3d(0,0,0), false);
////    human->setExternalTorque("lUpperLeg", Vector3d(0,0,0), false);
////    human->setExternalTorque("lLowerLeg", Vector3d(0,0,0), false);
////    human->setExternalTorque("rUpperLeg", Vector3d(0,0,0), false);
////    human->setExternalTorque("rLowerLeg", Vector3d(0,0,0), false);
////    human->setExternalTorque("lFoot", Vector3d(0,0,0), false);
////    human->setExternalTorque("rFoot", Vector3d(0,0,0), false);
////    human->setExternalTorque("lToes", Vector3d(0,0,0), false);
////    human->setExternalTorque("rToes", Vector3d(0,0,0), false);
//    
////    human->setOrientation("pelvis", Vector3d(-1,0,0), false);
////    human->setOrientation("lowerBack", Vector3d(-1,0,0), false);
////    human->setOrientation("torso", Vector3d(-1,0,0), false);
////    human->setOrientation("head", Vector3d(0,0,0), false);
////    human->setOrientation("lUpperArm", Vector3d(0,0,0), false);
////    human->setOrientation("rUpperArm", Vector3d(0,0,0), false);
////    human->setOrientation("lLowerArm", Vector3d(0,0,0), false);
////    human->setOrientation("rLowerArm", Vector3d(0,0,0), false);
////    human->setOrientation("lHand", Vector3d(0,0,0), false);
////    human->setOrientation("rHand", Vector3d(0,0,0), false);
////    human->setOrientation("lUpperLeg", Vector3d(-2,0,0), false);
////    human->setOrientation("lLowerLeg", Vector3d(-2,0,0), false);
////    human->setOrientation("rUpperLeg", Vector3d(-2,0,0), false);
////    human->setOrientation("rLowerLeg", Vector3d(-2,0,0), false);
////    human->setOrientation("lFoot", Vector3d(-2.5,0,0), false);
////    human->setOrientation("rFoot", Vector3d(-2.5,0,0), false);
////    human->setOrientation("lToes", Vector3d(-2.5,0,0), false);
////    human->setOrientation("rToes", Vector3d(-2.5,0,0), false);

    
    
    
    
//    human->setLock("pelvis", true, true);
//    human->setLock("lowerBack", true, false);
//    human->setLock("torso", true, true);
//    human->setLock("lUpperLeg", true, true);
//    human->setLock("lLowerLeg", true, true);
//    human->setLock("rLowerLeg", true, true);
//    human->setLock("rUpperLeg", true, true);
//    human->setExternalTorque("rUpperLeg", Vector3d(-2,0,0), false);
//    human->setAngVelocity("rUpperLeg", Vector3d(-60,0,0), false);
//    human->setLock("rUpperLeg", true, true);
//    nFrame = nFrame + 0.001;
//    printf("angle: %f\n", nFrame);
//    human->setOrientation("rUpperLeg", Vector3d(-nFrame,0,0), false);
//    human->setAngVelocity("rUpperLeg", Vector3d(-2,0,0), false);
//    nFrame += 0.2;
//    human->setOrientation("rUpperLeg", Vector3d(-nFrame,0,0), true);
//    human->setOrientation("rUpperLeg", Vector3d(0.5,0.5,0.5), false);
    
    double shoulderR[][3] = {{1.58, 1, 1.08}, {1.58, 1, 1.08}};
    double elbowR[][3] = {{-0.4, 0.1, -2.13}, {-0.4, -0.9, -2.13}};
    double shoulderL[][3] = {{-1.58, -1, -1.08}, {-1.58, -1, -1.08}};
    double elbowL[][3] = {{2.9, 0.1, -2.13}, {2.9, -0.9, -2.13}};
    double nan = std::numeric_limits<double>::quiet_NaN();
    int nF = 0;
//    if (human->getFootTouchedGround()==true)
//        printf("Foot on Ground!!!, %f\n", time);
    
    nSteps++;
    if (nSteps == SimGlobals::steps_per_second/2) {
        nSteps = 0;             
//        human->setExternalTorque("rUpperLeg", Vector3d(2,0,0), false);
//        human->setAngVelocity("rUpperLeg", Vector3d(-2,0,0), false);
//        nFrame = nFrame + 0.01;
//        printf("angle: %f\n", nFrame);
//        human->setOrientation("rUpperLeg", Vector3d(-nFrame,0,0), false);
        nF = nFrame;
                
        
        if(strcmp(sHand.c_str(), "Left")==0 || strcmp(sHand.c_str(), "Both")==0) {
            bcontroller->requestShoulderBend(shoulderL[nFrame][0], nan, shoulderL[nFrame][1], nan, shoulderL[nFrame][2], nan);
            bcontroller->requestElbowBend(elbowL[nFrame][0], nan, elbowL[nFrame][1], nan, elbowL[nFrame][2], nan);
        }
        if(strcmp(sHand.c_str(), "Right")==0 || strcmp(sHand.c_str(), "Both")==0) {
            bcontroller->requestShoulderBend(nan, shoulderR[nFrame][0], nan, shoulderR[nFrame][1], nan, shoulderR[nFrame][2]);
            bcontroller->requestElbowBend(nan, elbowR[nFrame][0], nan, elbowR[nFrame][1], nan, elbowR[nFrame][2]);
        }        
        nFrame++;
        if (nFrame >= 2)
            nFrame = 0;
    }
}

void WaveHand::onFinish() {
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
    cw->setController(humanName, 0);
    printf("Ending WavingHand Time: %f\n", time);
}

