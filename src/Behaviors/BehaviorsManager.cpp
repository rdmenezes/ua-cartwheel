/* 
 * File:   BehaviorsManager.cpp
 * Author: alfredo
 * 
 * Created on June 20, 2011, 10:24 AM
 */

#include "Behaviors/BehaviorsManager.h"
#include <Core/CartWheel3D.h>

using namespace std;
using namespace CartWheel;
using namespace CartWheel::Behaviors;

BehaviorsManager::BehaviorsManager() {
    populateBehaviorsMap();
}

BehaviorsManager::~BehaviorsManager() {
    _behaviors.clear();
    _behaviorCounts.clear();
    _mBehaviorsID.clear();
    _humanTime.clear();
}

void BehaviorsManager::reset() {
    _isBehaviorsDone = false;
    _serialTime = 0;
    _humanTime.clear();
}

double BehaviorsManager::getTotalSerialTime() {
    return _serialTime;
}

void BehaviorsManager::createSerialBehavior(string behaviorName, string humanName, Behaviors::Params* params, CartWheel3D* cw) {
    if(_humanTime.find(humanName.c_str()) != _humanTime.end()) {        
        params->startTime = _humanTime[humanName];
        _humanTime[humanName] += params->duration;
    } else {
        _humanTime[humanName] = params->duration;
    }    
    
//    printf("Calling Serial Behaviors: %s, %s, StartTime=%f, Duration=%f\n", behaviorName.c_str(), humanName.c_str(), _serialTime, params->duration);
    printf("Calling Serial Behaviors: %s, %s, StartTime=%f, Duration=%f\n", behaviorName.c_str(), humanName.c_str(), params->startTime, (double)_humanTime[humanName]);
    _serialTime += params->duration;
    createBehavior(behaviorName, humanName, params, cw);
}

void BehaviorsManager::createBehavior(string behaviorName, string humanName, Behaviors::Params* params, CartWheel3D* cw) {
    int nBehaviorID = findBehaviorID(behaviorName, humanName);
    string sBeh = boost::lexical_cast<string > (_behaviorCounts[humanName][behaviorName]);
    if (nBehaviorID < 0) {
        printf("Behavior (%s) or Human (%s) NOT found!!\n", behaviorName.c_str(), humanName.c_str());
        return;
    }

    switch (nBehaviorID) {
        case 1:
        {
            printf("Standing!!!\n");
            Standing* standing = new Standing(cw, humanName, dynamic_cast<Behaviors::Standing_Params*> (params));
            _behaviors[humanName][behaviorName.append(sBeh)] = dynamic_cast<Behaviors::Behavior*> (standing);
            break;
        }
        case 2:
        {
            printf("Push!!!\n");
            Push* push = new Push(cw, humanName, dynamic_cast<Behaviors::Push_Params*> (params));
            _behaviors[humanName][behaviorName.append(sBeh)] = dynamic_cast<Behaviors::Behavior*> (push);
            break;
        }
        case 3:
        {
            printf("Kick!!!\n");
            Kick* kick = new Kick(cw, humanName, dynamic_cast<Behaviors::Kick_Params*> (params));
            _behaviors[humanName][behaviorName.append(sBeh)] = dynamic_cast<Behaviors::Behavior*> (kick);
            break;
        }
        case 4:
        {
            printf("WalkInArc!!!\n");
            WalkInArc* walkArc = new WalkInArc(cw, humanName, dynamic_cast<Behaviors::WalkInArc_Params*> (params));
            _behaviors[humanName][behaviorName.append(sBeh)] = dynamic_cast<Behaviors::Behavior*> (walkArc);
            break;
        }
        case 5:
        {
            printf("WalkInPath!!!\n");
            WalkInPath* walkPath = new WalkInPath(cw, humanName, dynamic_cast<Behaviors::WalkInPath_Params*> (params));
            _behaviors[humanName][behaviorName.append(sBeh).c_str()] = dynamic_cast<Behaviors::Behavior*> (walkPath);
            break;
        }
        case 6:
        {
            printf("Walk!!!\n");
            Walk* walk = new Walk(cw, humanName, dynamic_cast<Behaviors::Walk_Params*> (params));
            printf("--Adding behavior: %s\n", behaviorName.append(sBeh).c_str());
            _behaviors[humanName][behaviorName.append(sBeh).c_str()] = dynamic_cast<Behaviors::Behavior*> (walk);
            break;
        }
        case 7:
        {
            printf("WaveHand!!!\n");
            WaveHand* waveHand = new WaveHand(cw, humanName, dynamic_cast<Behaviors::WaveHand_Params*> (params));
            _behaviors[humanName][behaviorName.append(sBeh).c_str()] = dynamic_cast<Behaviors::Behavior*> (waveHand);
            break;
        }
        case 8:
        {
            printf("MoveObject!!!\n");
            MoveObject* moveObj = new MoveObject(cw, humanName, dynamic_cast<Behaviors::MoveObject_Params*> (params));
            _behaviors[humanName][behaviorName.append(sBeh).c_str()] = dynamic_cast<Behaviors::Behavior*> (moveObj);
            break;
        }
        case 9:
            printf("Moving hand by IK\n");
            //            HumanoidIKCharacter humanIK(_humans[humanName]->getCharacter());
            ////            Point3d pos = _world->getRBByName("ball1")->getCMPosition();
            ////            printf("Target: %f, %f, %f\n", pos.x, pos.y, pos.z);
            //
            //            Vector3d shoulderL, shoulderR;
            //            double elbowL, elbowR;
            //            //////        humanIK.setLeftArmTarget(Point3d(0.1, 1.1, -2.3), &shoulderL, &elbowL);
            //            humanIK.setRightArmTarget(Point3d(0.1, 1.1, -2.3), &shoulderR, &elbowR);
            //
            //            _humans[humanName]->getBehaviour()->requestShoulderAngles(make_pair(shoulderL.x, shoulderR.x),
            //                    make_pair(shoulderL.y, shoulderR.y), make_pair(shoulderL.z, shoulderR.z));
            //            _humans[humanName]->getBehaviour()->requestElbowBend(elbowL, elbowR);
            //            //////
            //            //////        double leanS, leanC, twist;
            //            //////        humanIK.setSpineTarget(Point3d(0.1, 1.1, -2.3), &leanS, &leanC, &twist);
            //            //////        _humans[humanName]->getBehaviour()->requestUpperBodyPose(leanS, leanC, 0);
            //
            //            //        _humans[humanName]->getBehaviour()->requestKneeBend(PI*0.34);
            //            //        _humans[humanName]->getBehaviour()->alternateFootTraj
            //            //        humanIK.setRightLegTarget(Point3d(0, 0.1, -3));
            break;
    }
}

void BehaviorsManager::populateBehaviorsMap() {
    _mBehaviorsID["Standing"] = 1;
    _mBehaviorsID["Push"] = 2;
    _mBehaviorsID["Kick"] = 3;
    _mBehaviorsID["WalkInArc"] = 4;
    _mBehaviorsID["WalkInPath"] = 5;
    _mBehaviorsID["Walk"] = 6;
    _mBehaviorsID["WaveHand"] = 7;
    _mBehaviorsID["MoveObject"] = 8;
    _mBehaviorsID["MoveHand_IK"] = 9;
}

int BehaviorsManager::findBehaviorID(const std::string& behaviorName, const std::string& humanName) {
    if (_mBehaviorsID.find(behaviorName) != _mBehaviorsID.end()) {
        if (_behaviorCounts[humanName].find(behaviorName.c_str()) != _behaviorCounts[humanName].end()) {
            _behaviorCounts[humanName][behaviorName] += 1;
        } else {
            _behaviorCounts[humanName][behaviorName] = 1;
        }
        return _mBehaviorsID[behaviorName];
    } else {
        return -1;
    }
}

bool BehaviorsManager::isBehaviorsDone() {
    return _isBehaviorsDone;
}

void BehaviorsManager::setBehaviorsDone(bool isBehaviorsDone) {
    _isBehaviorsDone = isBehaviorsDone;
}

bool BehaviorsManager::runStep(std::string name, double _nTime) {
    bool isBehaviorsWorking = false;
    for (BehaviorsItr bItr = _behaviors[name].begin();
            bItr != _behaviors[name].end(); bItr++) {

        if ((*bItr).second->runStep(_nTime) == true) {
            isBehaviorsWorking = true;
        }
    }
    return isBehaviorsWorking;
}