/* 
 * File:   Params.h
 * Author: alfredo
 *
 * Created on June 3, 2011, 12:37 AM
 */

#ifndef PARAMS_H
#define	PARAMS_H

#include <string>
#include <MathLib/Trajectory.h>
using namespace CartWheel::Math;

namespace CartWheel {
    namespace Behaviors {

        class Params {
        public:
            double startTime;
            double duration;
            virtual ~Params() {}
        };
        
        class Kick_Params : public Params {
        public:
            Kick_Params(double startTime, double duration) {
                Params::startTime = startTime;
                Params::duration = duration;
            }
        };
        
        class Dig_Params : public Params {
        public:
            Dig_Params(double startTime, double duration) {
                Params::startTime = startTime;
                Params::duration = duration;
            }
        };
        
        class Push_Params : public Params {
        public:
            Push_Params(double startTime, double duration) {
                Params::startTime = startTime;
                Params::duration = duration;
            }
        };
        
        class Standing_Params : public Params {
        public:
            Standing_Params(double startTime, double duration) {
                Params::startTime = startTime;
                Params::duration = duration;
            }
        };
        
        class WalkInArc_Params : public Params {
        public:
            double speed;
            double angSpeed;
            
            WalkInArc_Params(double startTime, double duration, double speed, double angSpeed) {
                Params::startTime = startTime;
                Params::duration = duration;
                this->speed = speed;
                this->angSpeed = angSpeed;
            }
        };
        
        class WalkInPath_Params : public Params {
        public:
            double angSpeed;
            Trajectory3d* tPath;
            
            WalkInPath_Params(double startTime, double duration, double angSpeed, CartWheel::Math::Trajectory3d* tPath) {
                Params::startTime = startTime;
                Params::duration = duration;
                this->tPath = tPath;
                this->angSpeed = angSpeed;
            }
        };
        
        class Walk_Params : public Params {
        public:
            double speed;
            double angle;
            
            Walk_Params(double startTime, double duration, double speed, double angle) {
                Params::startTime = startTime;
                Params::duration = duration;
                this->speed = speed;
                this->angle = angle;
            }
        };
        
        class MoveObject_Params : public Params {
        public:
            Vector3d speed;
            Vector3d angSpeed;
            Vector3d orientation;
            Point3d position;            
            
            MoveObject_Params(double startTime, double duration, Point3d position, Vector3d orientation, Vector3d speed, Vector3d angSpeed) {
                Params::startTime = startTime;
                Params::duration = duration;
                this->speed = speed;
                this->orientation = orientation;
                this->position = position;
                this->angSpeed = angSpeed;
            }
        };
        
        class WaveHand_Params : public Params {
        public:
            std::string sHand;
            
            WaveHand_Params(double startTime, double duration, std::string hand) {
                Params::startTime = startTime;
                Params::duration = duration;
                this->sHand = hand.c_str();
            }
        };
        
        class Throw_Params : public Params {
        public:
            std::string sHand;
            
            Throw_Params(double startTime, double duration, std::string hand) {
                Params::startTime = startTime;
                Params::duration = duration;
                this->sHand = hand.c_str();
            }
        };
        
        class PickUp_Params : public Params {
        public:
            std::string sTargetObj;
            std::string sHand;
            
            PickUp_Params(double startTime, double duration, std::string sTargetObj, std::string hand) {
                Params::startTime = startTime;
                Params::duration = duration;
                this->sTargetObj = sTargetObj;
                this->sHand = hand;
            }
        };
    }
}

#endif	/* PARAMS_H */

