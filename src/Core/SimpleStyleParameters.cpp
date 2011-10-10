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
	headBendX = 0;
	headBendY = 0;
	headBendZ = 0;
	pelvisBendX = 0;
	pelvisBendY = 0;
	pelvisBendZ = 0;
	torsoBendX = 0;
	torsoBendY = 0;
	torsoBendZ = 0;
	elbowBendX = make_pair(0, 0);
	elbowBendY = make_pair(0, 0);
	elbowBendZ = make_pair(0, 0);
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
	bc->requestElbowAngles(elbowBendX, elbowBendY, elbowBendZ);
	bc->requestShoulderAngles(shoulderTwist, shoulderCoronal, shoulderSagittal);
	bc->requestPelvisTorsoBend(pelvisBendX, pelvisBendY, pelvisBendZ, torsoBendX, torsoBendY, torsoBendZ);
	bc->requestHeadBend(headBendX, headBendY, headBendZ);
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

	interp.elbowBendX.first = t * elbowBendX.first + (1-t) * other->elbowBendX.first;
	interp.elbowBendX.second = t * elbowBendX.second + (1-t) * other->elbowBendX.second;

	interp.elbowBendY.first = t * elbowBendY.first + (1-t) * other->elbowBendY.first;
	interp.elbowBendY.second = t * elbowBendY.second + (1-t) * other->elbowBendY.second;

	interp.elbowBendZ.first = t * elbowBendZ.first + (1-t) * other->elbowBendZ.first;
	interp.elbowBendZ.second = t * elbowBendZ.second + (1-t) * other->elbowBendZ.second;

	interp.shoulderTwist.first = t * shoulderTwist.first + (1-t) * other->shoulderTwist.first;
	interp.shoulderTwist.second = t * shoulderTwist.second + (1-t) * other->shoulderTwist.second;

	interp.shoulderCoronal.first = t * shoulderCoronal.first + (1-t) * other->shoulderCoronal.first;
	interp.shoulderCoronal.second = t * shoulderCoronal.second + (1-t) * other->shoulderCoronal.second;

	interp.shoulderSagittal.first = t * shoulderSagittal.first + (1-t) * other->shoulderSagittal.first;
	interp.shoulderSagittal.second = t * shoulderSagittal.second + (1-t) * other->shoulderSagittal.second;
        
	interp.pelvisBendX = t * pelvisBendX + (1-t) * other->pelvisBendX;
	interp.pelvisBendY = t * pelvisBendY + (1-t) * other->pelvisBendY;
	interp.pelvisBendZ = t * pelvisBendZ + (1-t) * other->pelvisBendZ;
        
	interp.torsoBendX = t * torsoBendX + (1-t) * other->torsoBendX;
	interp.torsoBendY = t * torsoBendY + (1-t) * other->torsoBendY;
	interp.torsoBendZ = t * torsoBendZ + (1-t) * other->torsoBendZ;
        
	interp.headBendX = t * headBendX + (1-t) * other->headBendX;
	interp.headBendY = t * headBendY + (1-t) * other->headBendY;
	interp.headBendZ = t * headBendZ + (1-t) * other->headBendZ;

	interp.applyStyleParameters(bc);
}
