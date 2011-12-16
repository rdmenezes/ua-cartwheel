/* 
 * File:   BehaviorsManager.h
 * Author: alfredo
 *
 * Created on June 20, 2011, 1:25 PM
 */

#ifndef BEHAVIORSMANAGER_H
#define	BEHAVIORSMANAGER_H

#include <string>
#include <map>
#include <boost/lexical_cast.hpp>

#include <Behaviors/Params.h>
#include <Behaviors/Behavior.h>
#include <Behaviors/Standing.h>
#include <Behaviors/Push.h>
#include <Behaviors/Kick.h>
#include <Behaviors/WalkInArc.h>
#include <Behaviors/Walk.h>
#include <Behaviors/WalkInPath.h>
#include <Behaviors/WaveHand.h>
#include <Behaviors/MoveObject.h>
#include <Behaviors/PickUp.h>
#include <Behaviors/Dig.h>
#include <Behaviors/Throw.h>
#include <Behaviors/SitDown.h>
#include <Behaviors/Jump.h>
#include <Behaviors/GiveObj.h>
#include <Behaviors/Falling.h>
#include <Behaviors/Catch.h>
#include <Behaviors/HandShake.h>


namespace CartWheel { 
    class CartWheel3D;
    namespace Behaviors {        
        class BehaviorsManager {
        private:
            typedef std::map<std::string /* BehaviorName */, Behaviors::Behavior*> BehaviorsMap;
            typedef std::map<std::string /* BehaviorName */, Behaviors::Behavior*>::iterator BehaviorsItr;
            typedef std::map<std::string /* BehaviorName */, int /* nBehaviors */> BehaviorsCounter;
            std::map<std::string /* HumanName */, BehaviorsMap> _behaviors;
            std::map<std::string /* HumanName */, BehaviorsCounter> _behaviorCounts;
            std::map<std::string /* HumanName */, double /* nHumanTime */> _humanTime;
            bool _isBehaviorsDone;
            double _serialTime, _parallelTime;
            std::map<std::string /* BehaviorName */, int /* BehaviorID */> _mBehaviorsID;
            
        public:
            BehaviorsManager();
            ~BehaviorsManager();
            
            void createBehavior(std::string behaviorName, std::string humanName, Behaviors::Params* params, CartWheel3D* cw);
            void createSerialBehavior(std::string behaviorName, std::string humanName, Behaviors::Params* params, CartWheel3D* cw);
            void createParallelBehavior(std::string behaviorName, std::string humanName, Behaviors::Params* params, CartWheel3D* cw);
            bool runStep(std::string name, double _nTime);
            void populateBehaviorsMap();
            int findBehaviorID(const std::string& behaviorName, const std::string& humanName);
            void reset();
            
            bool isBehaviorsDone();
            void setBehaviorsDone(bool isBehaviorsDone);
            double getTotalSerialTime();
            double getTotalParallelTime();
        };
    }
}

#endif	/* BEHAVIORSMANAGER_H */

