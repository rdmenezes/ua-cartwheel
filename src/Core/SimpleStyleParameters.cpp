#include <Core/SimpleStyleParameters.h>
#include <Core/SimGlobals.h>

using namespace CartWheel;
using namespace CartWheel::Core;
using namespace CartWheel::Physics;
using namespace CartWheel::Math;
using namespace CartWheel::Util;

using namespace std;

SimpleStyleParameters::SimpleStyleParameters(void){
	ubSagittalLean = 0;
	ubCoronalLean = 0;
	ubTwist = 0;
	velDSagittal = 0;
	velDCoronal = 0;
	kneeBend = 0;
	duckFootedness = 0;
	stepHeight = 0;
	coronalStepWidth = 0;
	elbowBend = make_pair(0, 0);
	shoulderTwist = make_pair(0, 0);
	shoulderCoronal = make_pair(0, 0);
	shoulderSagittal = make_pair(0, 0);
}

SimpleStyleParameters::~SimpleStyleParameters(void){

}

void SimpleStyleParameters::applyStyleParameters(BehaviourController* bc){
	bc->requestUpperBodyPose(ubSagittalLean, ubCoronalLean, ubTwist);
	bc->requestVelocities(velDSagittal, velDCoronal);
	bc->requestKneeBend(kneeBend);
	bc->requestDuckFootedness(duckFootedness);
	bc->requestStepHeight(stepHeight);

	bc->setElbowAngles(elbowBend.first, elbowBend.second);
	bc->setShoulderAngles(shoulderTwist.first, shoulderTwist.first, shoulderCoronal.first,
			shoulderCoronal.second, shoulderSagittal.second, shoulderSagittal.second);
}

/**
	if t=1, it's all this style. If it's 0, it's all the other...
*/
void SimpleStyleParameters::applyInterpolatedStyleParameters(BehaviourController* bc, double t, SimpleStyleParameters* other){
	boundToRange(&t, 0, 1);
	if (IS_ZERO(t-1) || other == NULL) applyStyleParameters(bc);
	if (IS_ZERO(t)) other->applyStyleParameters(bc);

	SimpleStyleParameters interp;
	interp.ubSagittalLean = t * ubSagittalLean + (1-t) * other->ubSagittalLean;
	interp.ubCoronalLean = t * ubCoronalLean + (1-t) * other->ubCoronalLean;
	interp.ubTwist = t * ubTwist + (1-t) * other->ubTwist;

	interp.velDSagittal = t * velDSagittal + (1-t) * other->velDSagittal;
	interp.velDCoronal = t * velDCoronal + (1-t) * other->velDCoronal;

	interp.kneeBend = t * kneeBend + (1-t) * other->kneeBend;
	interp.duckFootedness = t * duckFootedness + (1-t) * other->duckFootedness;
	interp.stepHeight = t * stepHeight + (1-t) * other->stepHeight;

	interp.coronalStepWidth = t * coronalStepWidth + (1-t) * other->coronalStepWidth;

	interp.elbowBend.first = t * elbowBend.first + (1-t) * other->elbowBend.first;
	interp.elbowBend.second = t * elbowBend.second + (1-t) * other->elbowBend.second;

	interp.shoulderTwist.first = t * shoulderTwist.first + (1-t) * other->shoulderTwist.first;
	interp.shoulderTwist.second = t * shoulderTwist.second + (1-t) * other->shoulderTwist.second;

	interp.shoulderCoronal.first = t * shoulderCoronal.first + (1-t) * other->shoulderCoronal.first;
	interp.shoulderCoronal.second = t * shoulderCoronal.second + (1-t) * other->shoulderCoronal.second;

	interp.shoulderSagittal.first = t * shoulderSagittal.first + (1-t) * other->shoulderSagittal.first;
	interp.shoulderSagittal.second = t * shoulderSagittal.second + (1-t) * other->shoulderSagittal.second;

	interp.applyStyleParameters(bc);
}
