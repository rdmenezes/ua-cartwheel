#include "Behaviors/Catch.h"
#include <Core/CartWheel3D.h>

using namespace CartWheel;
using namespace CartWheel::Core;

Catch::Catch(CartWheel3D* cw, std::string humanName, Catch_Params* params)
        : Behavior(cw, humanName, params!=NULL ? params->startTime : 0, params!=NULL ? params->duration : 0) {
    this->sTargetObj = params->sTargetObj;
    this->sHand = params->sHand.c_str();
    this->cw->getHuman(humanName, &human);
    bcontroller = human->getBehaviour();
    bCatchFinished = false;
}

void Catch::onInit() {
    printf("starting to Catch\n");
}

void Catch::runStep() {
//    human->setLock("rHand", true, true);
//    human->setVelocity("rHand", Vector3d(0,1,0));
    double nan = std::numeric_limits<double>::quiet_NaN();
    nSteps++;
    if (nSteps == SimGlobals::steps_per_second/2 && !bCatchFinished) {
        nSteps = 0;        
        
        Vector3d shoulderL, shoulderR, elbowL, elbowR, handR, handL, pelvis, head, torso;
        Physics::RigidBody* obj = cw->getObjectByName(sTargetObj.c_str());
               
        Point3d posObj = obj->getCMPosition();
//        posObj.y += 0.05;
//        posObj += 0.05;
//        posObj.x += 0.05;
//        posObj.z += 0.05;
//        Point3d posObj = Point3d(0, 0.1, -1);
        Point3d posHuman;
        
        if(strcmp(sHand.c_str(), "Left")==0) {
            human->getLeftArmIK(posObj, &shoulderL, &elbowL, &handL);
            shoulderR.x = nan; shoulderR.y = nan; shoulderR.z = nan;
            elbowR.x = nan; elbowR.y = nan; elbowR.z = nan;
            handR.x = nan; handR.y = nan; handR.z = nan;
            posHuman = human->getCharacter()->getARBByName("lHand")->getCMPosition();
        } else if(strcmp(sHand.c_str(), "Right")==0) {
            human->getRightArmIK(posObj, &shoulderR, &elbowR, &handR);
            shoulderL.x = nan; shoulderL.y = nan; shoulderL.z = nan;
            elbowL.x = nan; elbowL.y = nan; elbowL.z = nan;
            handL.x = nan; handL.y = nan; handL.z = nan;
            posHuman = human->getCharacter()->getARBByName("rHand")->getCMPosition();
        } else if(strcmp(sHand.c_str(), "Both")==0) {
            human->getLeftArmIK(posObj, &shoulderL, &elbowL, &handL);
            human->getRightArmIK(posObj, &shoulderR, &elbowR, &handR);
            posHuman = (human->getCharacter()->getARBByName("lHand")->getCMPosition() + 
                    human->getCharacter()->getARBByName("rHand")->getCMPosition());
            posHuman /= 2;
        }
        human->getPelvisTorsoIK(posObj, &pelvis, &torso);
        human->getHeadIK(posObj, &head);
        
        double bendOverK = getBendOverK(posObj);
        pelvis.x = pelvis.x < 0 ? 0 : pelvis.x;
        torso.x = torso.x < 0 ? 0 : torso.x;
        bcontroller->requestShoulderBend(shoulderL.x, shoulderR.x, shoulderL.y, shoulderR.y, shoulderL.z, shoulderR.z);
        bcontroller->requestElbowBend(elbowL.x, elbowR.x, elbowL.y, elbowR.y, elbowL.z, elbowR.z);
        bcontroller->requestHandBend(handL.x, handR.x, handL.y, handR.y, handL.z, handR.z);
        if(bendOverK > 0)
            bcontroller->requestPelvisTorsoBend(pelvis.x, pelvis.y, 0, torso.x, torso.y, 0);
        bcontroller->requestHeadBend(0, head.y, 0);
        bcontroller->requestKneeBend(bendOverK);
        
        double distTarget = getDistXYZ(posObj, posHuman);
        bool heightReached = (bendOverK > 0.9) && (distTarget < 0.58);
        if(heightReached || distTarget < 1) {  //closer: distTarget < 0.228        0.25     current:0.23
            printf("Distance: %f, Object Grabbing!!!\n", distTarget);
            if(strcmp(sHand.c_str(), "Left")==0)
                cw->makeHumanGrabObject(humanName, sTargetObj.c_str(), Human::left);
            else if(strcmp(sHand.c_str(), "Right")==0)
                cw->makeHumanGrabObject(humanName, sTargetObj.c_str(), Human::right);
            else if(strcmp(sHand.c_str(), "Both")==0)
                cw->makeHumanGrabObject(humanName, sTargetObj.c_str(), Human::both);
            resetHumanAngles();
        } else {
            printf("Distance: %f, BendOver: %f\n", distTarget, bendOverK);
        }
    }
    if (bCatchFinished) {
//        human->setLock("lHand", true, true);
//        human->setLock("rHand", true, true);
        human->setLock("torso", true, true);
//        human->setLock("lowerBack", true, true);
        human->setLock("pelvis", true, true);
//        printf("Velocity: %f, %f, %f\n", cw->getObjectByName("boxHandle2 boxH")->getCMVelocity().x, 
//                cw->getObjectByName("boxHandle2 boxH")->getCMVelocity().y, 
//                cw->getObjectByName("boxHandle2 boxH")->getCMVelocity().z);
////        cw->getObjectByName("boxHandle2 boxH")->setCMVelocity(Vector3d(0,0,0));
        cw->getObjectByName("ball1")->setCMVelocity(Vector3d(0,0,0));
    }
}

void Catch::onFinish() {
    printf("Ending Catch Time: %f\n", time);
    resetHumanAngles();
}

double Catch::getBendOverK(Point3d pObj) {
    Point3d pHuman = human->getPosition();
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

double Catch::getDistXZ(Point3d p1, Point3d p2) {
    double d = sqrt(pow(p1.x - p2.x, 2) + pow(p1.z - p2.z, 2));
    return d;
}

double Catch::getDistXYZ(Point3d p1, Point3d p2) {
    double d = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) + pow(p1.z - p2.z, 2));
    return d;
}

void Catch::resetHumanAngles() {
    bcontroller->requestHeadBend(0, 0, 0);
    bcontroller->requestPelvisTorsoBend(0, 0, 0, 0, 0, 0);
    bcontroller->requestShoulderBend(0, 0, 0, 0, -1.5, 1.5);
    human->setLock("lHand", true, true);
    human->setLock("rHand", true, true);
    
    if(strcmp(sHand.c_str(), "Both")==0) {
        bcontroller->requestElbowBend(0, 0, -1, 1, 0, 0);
        bcontroller->requestHandBend(PI*0.6, PI*0.5, 0, 0, 0, 0);
    } else {
        bcontroller->requestHandBend(0,0, 0,0, 0,0);
//        bcontroller->requestHandBend(-1.5,1.5, 0,0, 0,0);
//        human->factorAngVelocity("lHand", Vector3d(0.4,0.4,0.4));
//        human->factorAngVelocity("rHand", Vector3d(0.4,0.4,0.4));
//        printf("Reseting Hands\n");
//        human->setOrientation("lHand", Vector3d(0.0,0.0,0.0));
//        human->setOrientation("rHand", Vector3d(0.0,0.0,0.0));
    }
    bcontroller->requestKneeBend(0);   
    bCatchFinished = true;
}