#pragma once

#include <Core/BehaviourController.h>

namespace CartWheel { namespace Core {

/**
	container for parameter settings for simple walking styles
*/
class SimpleStyleParameters{
public:
	double ubSagittalLean;
	double ubCoronalLean;
	double ubTwist;
	double velDSagittal;
	double velDCoronal;
	double kneeBend;
	double duckFootedness;
	double stepHeight;
	double coronalStepWidth;
	BehaviourController::LeftRightDouble elbowBend;
	BehaviourController::LeftRightDouble shoulderTwist;
	BehaviourController::LeftRightDouble shoulderCoronal;
	BehaviourController::LeftRightDouble shoulderSagittal;

public:
	SimpleStyleParameters(void);
	~SimpleStyleParameters(void);

	void applyStyleParameters(BehaviourController* bc);

	/**
		if t=1, it's all this style. If it's 0, it's all the other...
	*/
	void applyInterpolatedStyleParameters(BehaviourController* bc, double t, SimpleStyleParameters* other);

};

}
}

