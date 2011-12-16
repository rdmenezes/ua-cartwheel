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

double BehaviorsManager::getTotalParallelTime() {
    return _parallelTime;
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

void BehaviorsManager::createParallelBehavior(string behaviorName, string humanName, Behaviors::Params* params, CartWheel3D* cw) {
    if(_humanTime.find(humanName.c_str()) != _humanTime.end()) {        
        params->startTime = _humanTime[humanName];
        _humanTime[humanName] += params->duration;
    } else {
        _humanTime[humanName] = params->duration;
    }    
    
//    printf("Calling Parallel Behaviors: %s, %s, StartTime=%f, Duration=%f\n", behaviorName.c_str(), humanName.c_str(), _serialTime, params->duration);
    printf("Calling Parallel Behaviors: %s, %s, StartTime=%f, Duration=%f\n", behaviorName.c_str(), humanName.c_str(), params->startTime, (double)_humanTime[humanName]);
    _parallelTime += params->duration;
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
        {
            printf("PickUp!!!\n");
            PickUp* pickUp = new PickUp(cw, humanName, dynamic_cast<Behaviors::PickUp_Params*> (params));
            _behaviors[humanName][behaviorName.append(sBeh).c_str()] = dynamic_cast<Behaviors::Behavior*> (pickUp);
            break;
        }
        case 10:
        {
            printf("Dig!!!\n");
            Dig* dig = new Dig(cw, humanName, dynamic_cast<Behaviors::Dig_Params*> (params));
            _behaviors[humanName][behaviorName.append(sBeh).c_str()] = dynamic_cast<Behaviors::Behavior*> (dig);
            break;
        }
        case 11:
        {
            printf("Throw!!!\n");
            Throw* throwObj = new Throw(cw, humanName, dynamic_cast<Behaviors::Throw_Params*> (params));
            _behaviors[humanName][behaviorName.append(sBeh).c_str()] = dynamic_cast<Behaviors::Behavior*> (throwObj);
            break;
        }
        case 12:
        {
            printf("SitDown!!!\n");
            SitDown* sitDown = new SitDown(cw, humanName, dynamic_cast<Behaviors::SitDown_Params*> (params));
            _behaviors[humanName][behaviorName.append(sBeh).c_str()] = dynamic_cast<Behaviors::Behavior*> (sitDown);
            break;
        }
        case 13:
        {
            printf("Jump!!!\n");
            Jump* jump = new Jump(cw, humanName, dynamic_cast<Behaviors::Jump_Params*> (params));
            _behaviors[humanName][behaviorName.append(sBeh).c_str()] = dynamic_cast<Behaviors::Behavior*> (jump);
            break;
        }
        case 14:
        {
            printf("GiveObj!!!\n");
            GiveObj* giveObj = new GiveObj(cw, humanName, dynamic_cast<Behaviors::GiveObj_Params*> (params));
            _behaviors[humanName][behaviorName.append(sBeh).c_str()] = dynamic_cast<Behaviors::Behavior*> (giveObj);
            break;
        }
        case 15:
        {
            printf("Falling!!!\n");
            Falling* falling = new Falling(cw, humanName, dynamic_cast<Behaviors::Falling_Params*> (params));
            _behaviors[humanName][behaviorName.append(sBeh).c_str()] = dynamic_cast<Behaviors::Behavior*> (falling);
            break;
        }
        case 16:
        {
            printf("Catch!!!\n");
            Catch* catching = new Catch(cw, humanName, dynamic_cast<Behaviors::Catch_Params*> (params));
            _behaviors[humanName][behaviorName.append(sBeh).c_str()] = dynamic_cast<Behaviors::Behavior*> (catching);
            break;
        }
        case 17:
        {
            printf("HandShake!!!\n");
            HandShake* handShake = new HandShake(cw, humanName, dynamic_cast<Behaviors::HandShake_Params*> (params));
            _behaviors[humanName][behaviorName.append(sBeh).c_str()] = dynamic_cast<Behaviors::Behavior*> (handShake);
            break;
        }
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
    _mBehaviorsID["PickUp"] = 9;
    _mBehaviorsID["Dig"] = 10;
    _mBehaviorsID["Throw"] = 11;
    _mBehaviorsID["SitDown"] = 12;
    _mBehaviorsID["Jump"] = 13;
    _mBehaviorsID["GiveObj"] = 14;
    _mBehaviorsID["Falling"] = 15;
    _mBehaviorsID["Catch"] = 16;
    _mBehaviorsID["HandShake"] = 17;
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