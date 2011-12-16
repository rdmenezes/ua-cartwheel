#include "Behaviors/HandShake.h"
#include <Core/CartWheel3D.h>

using namespace CartWheel;
using namespace CartWheel::Core;

HandShake::HandShake(CartWheel3D* cw, std::string humanName, HandShake_Params* params)
        : Behavior(cw, humanName, params!=NULL ? params->startTime : 0, params!=NULL ? params->duration : 0) {
    this->sFromHand = params->sFromHand.c_str();
    this->sToHand = params->sToHand.c_str();
    this->sToHuman = params->sToHuman.c_str();
    this->cw->getHuman(humanName, &human1);
    bcontroller1 = human1->getBehaviour();
    this->cw->getHuman(this->sToHuman, &human2);
    bcontroller2 = human2->getBehaviour();
    bHandShakeFinished = false;
    nFrame = 0;
}

void HandShake::onInit() {
    printf("starting to HandShake\n");
}

void HandShake::runStep() {
    double nan = std::numeric_limits<double>::quiet_NaN();
    double distTarget = 0;
    nSteps++;
    if (nSteps == SimGlobals::steps_per_second/2 && !bHandShakeFinished) {
        nSteps = 0;
        if(nFrame==0) {
            Point3d posHuman1 = moveIK_Human1();
            Point3d posHuman2 = moveIK_Human2();
            distTarget = getDistXYZ(posHuman1, posHuman2);
        }
        if(distTarget < 0.3) {  //closer: distTarget < 0.228        0.25 
            printf("Distance: %f, Object Grabbing!!!\n", distTarget);
            nFrame++;
            if (nFrame<=1) {
                if(strcmp(sToHand.c_str(), "Left")==0 && strcmp(sFromHand.c_str(), "Left")==0) {
                    bcontroller1->requestHandBend(nan,1.5, nan,nan, nan,nan);
                    bcontroller2->requestHandBend(nan,1.5, nan,nan, nan,nan);
                } else if(strcmp(sToHand.c_str(), "Right")==0 && strcmp(sFromHand.c_str(), "Right")==0) {
                    bcontroller1->requestHandBend(nan,-1.5, nan,nan, nan,nan);
                    bcontroller2->requestHandBend(nan,-1.5, nan,nan, nan,nan);
                } 
//                else if(strcmp(sToHand.c_str(), "Both")==0) {
//                    bcontroller2->requestHandBend(nan,-1.5, nan,nan, nan,nan);
//                } else if(strcmp(sFromHand.c_str(), "Both")==0) {
//                    bcontroller1->requestHandBend(nan,-1.5, nan,nan, nan,nan);
//                }
                printf("Shaking Hands!!!\n");
            } else {
                resetHumanAngles();
            }
        } else {
            printf("Distance: %f\n", distTarget);
        }
    }
    if (bHandShakeFinished) {
//        human->setLock("lHand", true, true);
//        human->setLock("rHand", true, true);
        human1->setLock("torso", true, true);
//        human->setLock("lowerBack", true, true);
        human1->setLock("pelvis", true, true);
//        printf("Velocity: %f, %f, %f\n", cw->getObjectByName("boxHandle2 boxH")->getCMVelocity().x, 
//                cw->getObjectByName("boxHandle2 boxH")->getCMVelocity().y, 
//                cw->getObjectByName("boxHandle2 boxH")->getCMVelocity().z);
////        cw->getObjectByName("boxHandle2 boxH")->setCMVelocity(Vector3d(0,0,0));
//        cw->getObjectByName("ball1")->setCMVelocity(Vector3d(0,0,0));
    }
}

void HandShake::onFinish() {
    printf("Ending HandShake Time: %f\n", time);
    resetHumanAngles();
}

Point3d HandShake::moveIK_Human1() {
    double nan = std::numeric_limits<double>::quiet_NaN();
    Point3d posObj, pObj1, pObj2;
    if(strcmp(sToHand.c_str(), "Left")==0) {
        posObj = human2->getCharacter()->getARBByName("lHand")->getCMPosition();
    } else if(strcmp(sToHand.c_str(), "Right")==0) {
        posObj = human2->getCharacter()->getARBByName("rHand")->getCMPosition();
    } else if(strcmp(sToHand.c_str(), "Both")==0) {
        pObj1 = human2->getCharacter()->getARBByName("lHand")->getCMPosition();
        pObj2 = human2->getCharacter()->getARBByName("rHand")->getCMPosition();
        posObj = (pObj1 + pObj2);
        posObj /= 2;
    }    
    std::string fromHand = sFromHand;
    Point3d posHuman;
        
    Vector3d shoulderL, shoulderR, elbowL, elbowR, handR, handL, pelvis, head, torso;
    if(strcmp(fromHand.c_str(), "Left")==0) {
        human1->getLeftArmIK(posObj, &shoulderL, &elbowL, &handL);
        shoulderR.x = nan; shoulderR.y = nan; shoulderR.z = nan;
        elbowR.x = nan; elbowR.y = nan; elbowR.z = nan;
        handR.x = nan; handR.y = nan; handR.z = nan;
        posHuman = human1->getCharacter()->getARBByName("lHand")->getCMPosition();
    } else if(strcmp(fromHand.c_str(), "Right")==0) {
        human1->getRightArmIK(posObj, &shoulderR, &elbowR, &handR);
        shoulderL.x = nan; shoulderL.y = nan; shoulderL.z = nan;
        elbowL.x = nan; elbowL.y = nan; elbowL.z = nan;
        handL.x = nan; handL.y = nan; handL.z = nan;
        posHuman = human1->getCharacter()->getARBByName("rHand")->getCMPosition();
    } else if(strcmp(fromHand.c_str(), "Both")==0) {
        human1->getLeftArmIK(posObj, &shoulderL, &elbowL, &handL);
        human1->getRightArmIK(posObj, &shoulderR, &elbowR, &handR);
        posHuman = (human1->getCharacter()->getARBByName("lHand")->getCMPosition() + 
                human1->getCharacter()->getARBByName("rHand")->getCMPosition());
        posHuman /= 2;
    }
    human1->getPelvisTorsoIK(posObj, &pelvis, &torso);
    human1->getHeadIK(posObj, &head);

    pelvis.x = pelvis.x < 0 ? 0 : pelvis.x;
    torso.x = torso.x < 0 ? 0 : torso.x;
    bcontroller1->requestShoulderBend(shoulderL.x, shoulderR.x, shoulderL.y, shoulderR.y, shoulderL.z, shoulderR.z);
    bcontroller1->requestElbowBend(elbowL.x, elbowR.x, elbowL.y, elbowR.y, elbowL.z, elbowR.z);
    bcontroller1->requestHandBend(handL.x, handR.x, handL.y, handR.y, handL.z, handR.z);
    bcontroller1->requestHeadBend(0, head.y, 0);
    
    return posHuman;
}

Point3d HandShake::moveIK_Human2() {
    double nan = std::numeric_limits<double>::quiet_NaN();
    Point3d posObj, pObj1, pObj2;
    if(strcmp(sFromHand.c_str(), "Left")==0) {
        posObj = human1->getCharacter()->getARBByName("lHand")->getCMPosition();
    } else if(strcmp(sFromHand.c_str(), "Right")==0) {
        posObj = human1->getCharacter()->getARBByName("rHand")->getCMPosition();
    } else if(strcmp(sFromHand.c_str(), "Both")==0) {
        pObj1 = human1->getCharacter()->getARBByName("lHand")->getCMPosition();
        pObj2 = human1->getCharacter()->getARBByName("rHand")->getCMPosition();
        posObj = (pObj1 + pObj2);
        posObj /= 2;
    }
    Point3d posHuman;
        
    Vector3d shoulderL, shoulderR, elbowL, elbowR, handR, handL, pelvis, head, torso;
    if(strcmp(sToHand.c_str(), "Left")==0) {
        human2->getLeftArmIK(posObj, &shoulderL, &elbowL, &handL);
        shoulderR.x = nan; shoulderR.y = nan; shoulderR.z = nan;
        elbowR.x = nan; elbowR.y = nan; elbowR.z = nan;
        handR.x = nan; handR.y = nan; handR.z = nan;
        posHuman = human2->getCharacter()->getARBByName("lHand")->getCMPosition();
    } else if(strcmp(sToHand.c_str(), "Right")==0) {
        human2->getRightArmIK(posObj, &shoulderR, &elbowR, &handR);
        shoulderL.x = nan; shoulderL.y = nan; shoulderL.z = nan;
        elbowL.x = nan; elbowL.y = nan; elbowL.z = nan;
        handL.x = nan; handL.y = nan; handL.z = nan;
        posHuman = human2->getCharacter()->getARBByName("rHand")->getCMPosition();
    } else if(strcmp(sToHand.c_str(), "Both")==0) {
        human2->getLeftArmIK(posObj, &shoulderL, &elbowL, &handL);
        human2->getRightArmIK(posObj, &shoulderR, &elbowR, &handR);
        posHuman = (human2->getCharacter()->getARBByName("lHand")->getCMPosition() + 
                human2->getCharacter()->getARBByName("rHand")->getCMPosition());
        posHuman /= 2;
    }
    human2->getPelvisTorsoIK(posObj, &pelvis, &torso);
    human2->getHeadIK(posObj, &head);

    pelvis.x = pelvis.x < 0 ? 0 : pelvis.x;
    torso.x = torso.x < 0 ? 0 : torso.x;
    bcontroller2->requestShoulderBend(shoulderL.x, shoulderR.x, shoulderL.y, shoulderR.y, shoulderL.z, shoulderR.z);
    bcontroller2->requestElbowBend(elbowL.x, elbowR.x, elbowL.y, elbowR.y, elbowL.z, elbowR.z);
    bcontroller2->requestHandBend(handL.x, handR.x, handL.y, handR.y, handL.z, handR.z);
    bcontroller2->requestHeadBend(0, head.y, 0);
    
    return posHuman;
}

double HandShake::getBendOverK(Point3d pObj) {
    Point3d pHuman = human1->getPosition();
    double nDist = getDistXZ(pObj, pHuman);
    double nHeightY = pHuman.y - pObj.y;
    double maxK = 1.4;
    double k = 0;
        
    if(nHeightY > 0.4 && nDist < 0.8) {
        k = 1/(1+(nDist))*maxK;
    } else {
        k = 0;
    }
    return k;
}

double HandShake::getDistXZ(Point3d p1, Point3d p2) {
    double d = sqrt(pow(p1.x - p2.x, 2) + pow(p1.z - p2.z, 2));
    return d;
}

double HandShake::getDistXYZ(Point3d p1, Point3d p2) {
    double d = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) + pow(p1.z - p2.z, 2));
    return d;
}

void HandShake::resetHumanAngles() {
    double nan = std::numeric_limits<double>::quiet_NaN();
    bcontroller1->requestHeadBend(0, 0, 0);
    bcontroller1->requestPelvisTorsoBend(0, 0, 0, 0, 0, 0);
    bcontroller1->requestShoulderBend(0, 0, 0, 0, -1.5, 1.5);
    human1->setLock("lHand", true, true);
    human1->setLock("rHand", true, true);
    bcontroller2->requestHeadBend(0, 0, 0);
    bcontroller2->requestPelvisTorsoBend(0, 0, 0, 0, 0, 0);
    bcontroller2->requestShoulderBend(0, 0, 0, 0, -1.5, 1.5);
    human2->setLock("lHand", true, true);
    human2->setLock("rHand", true, true);
    
    if(strcmp(sToHand.c_str(), "Both")==0) {
        bcontroller1->requestElbowBend(0, 0, -1, 1, 0, 0);
        bcontroller1->requestHandBend(PI*0.6, PI*0.5, 0, 0, 0, 0);
        bcontroller2->requestElbowBend(0, 0, -1, 1, 0, 0);
        bcontroller2->requestHandBend(PI*0.6, PI*0.5, 0, 0, 0, 0);
    } else {
        bcontroller1->requestHandBend(0,0, 0,0, 0,0);
        bcontroller1->requestElbowBend(0, 0, 0, 0, 0, 0);
        bcontroller2->requestHandBend(0,0, 0,0, 0,0);
        bcontroller2->requestElbowBend(0, 0, 0, 0, 0, 0);
//        bcontroller->requestHandBend(-1.5,1.5, 0,0, 0,0);
//        human->factorAngVelocity("lHand", Vector3d(0.4,0.4,0.4));
//        human->factorAngVelocity("rHand", Vector3d(0.4,0.4,0.4));
//        printf("Reseting Hands\n");
//        human->setOrientation("lHand", Vector3d(0.0,0.0,0.0));
//        human->setOrientation("rHand", Vector3d(0.0,0.0,0.0));
    }
    bcontroller1->requestKneeBend(0);   
    bcontroller2->requestKneeBend(0);  
    bHandShakeFinished = true;
}