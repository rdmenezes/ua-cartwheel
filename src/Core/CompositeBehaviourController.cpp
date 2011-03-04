/*
 * CompositeBehaviourController.cpp
 *
 *  Created on: Feb 28, 2011
 *      Author: denis
 */

#include <Core/CompositeBehaviourController.h>
#include <Core/TurnController.h>
#include <Core/DuckController.h>
#include <Core/SimpleStyleParameters.h>

#include <iostream>

using namespace CartWheel;
using namespace CartWheel::Core;
using namespace CartWheel::Math;
using namespace CartWheel::Util;

using namespace std;

inline int max(int a, int b) {
	return (a > b) ? a : b;
}

CompositeBehaviourController::CompositeBehaviourController(Character* b,
		IKVMCController* llc, WorldOracle* w) :
	BehaviourController(b, llc, w),
	activeBehaviour (0),
	phi (0.0 ){
}

void CompositeBehaviourController::loadFromFile(FILE * f) {
	if (f == NULL)
		throwError("Invalid file pointer.");
	//now we'll interpret the input file...

	//have a temporary buffer used to read the file line by line...
	char buffer[200];
	//this is where it happens.
	while (!feof(f)) {
		//get a line from the file...
		fgets(buffer, 200, f);
		if (feof(f))
			break;
		if (strlen(buffer) > 195)
			throwError("The input file contains a line that is longer than ~200 characters - not allowed");
		char *line = lTrim(buffer);
		int lineType = getConLineType(line);
		switch (lineType) {
		case CON_TURN_BEHAVIOUR: {
			//add a new controller
			BehaviourController* behaviour = new TurnController(bip, lowLCon, wo);
			behaviour->loadFromFile(f);
			controllers.push_back(behaviour);
			break;
		}
		case CON_DUCK_BEHAVIOUR: {
			//add a new controller
			BehaviourController* behaviour = new DuckController(bip, lowLCon, wo);
			behaviour->loadFromFile(f);
			controllers.push_back(behaviour);
			break;
		}
		case CON_STATE_TIME: {
			double duration = 0.0;
			if (sscanf(line, "%lf", &duration)!=1)
				throwError("The time that is expected to be spent in this state needs to be provided.");
			behaviourDurations.push_back(duration);
			break;
		}

		case CON_NOT_IMPORTANT:
			printf("Ignoring input line: \'%s\'\n", line);
			break;
		case CON_COMMENT:
			break;
		default:
			throwError("Incorrect behaviour input file: \'%s\' - unexpected line.",	buffer);
		}
	}
}

void CompositeBehaviourController::saveToFile(FILE * file) {

}

void CompositeBehaviourController::setUpperBodyPose(double leanSagittal,
		double leanCoronal, double twist) {
	controllers[activeBehaviour]->setUpperBodyPose(leanSagittal, leanCoronal, twist);
}

void CompositeBehaviourController::setKneeBend(double v, bool swingAlso) {
	controllers[activeBehaviour]->setKneeBend(v, swingAlso);
}

void CompositeBehaviourController::setDuckWalkDegree(double v) {
	controllers[activeBehaviour]->setDuckWalkDegree(v);
}

void CompositeBehaviourController::setDesiredHeading(double v) {
	for (unsigned int i = 0; i < controllers.size(); i++) {
		controllers[i]->setDesiredHeading(i);
	}
}

void CompositeBehaviourController::setVelocities(double velDS, double velDC) {
	controllers[activeBehaviour]->setVelocities(velDS, velDC);
}

void CompositeBehaviourController::adjustStepHeight() {
	controllers[activeBehaviour]->adjustStepHeight();
}

void CompositeBehaviourController::setElbowAngles(double leftElbowAngle, double rightElbowAngle) {
	controllers[activeBehaviour]->setElbowAngles(leftElbowAngle, rightElbowAngle);
}

void CompositeBehaviourController::setShoulderAngles(double leftTwist, double rightTwist, double leftAdduction,
		double rightAdduction, double leftSwing, double rightSwing) {
	controllers[activeBehaviour]->setShoulderAngles(leftTwist, rightTwist, leftAdduction, rightAdduction,
			leftSwing, rightSwing);
}

void CompositeBehaviourController::requestStepTime(double stepTime) {
	controllers[activeBehaviour]->requestStepTime(stepTime);
}

void CompositeBehaviourController::requestStepHeight(double stepHeight) {
	controllers[activeBehaviour]->requestStepHeight(stepHeight);
}

void CompositeBehaviourController::requestVelocities(double velDS, double velDC) {
	controllers[activeBehaviour]->requestVelocities(velDS, velDC);
}

void CompositeBehaviourController::requestUpperBodyPose(double leanS, double leanC, double twist) {
	controllers[activeBehaviour]->requestUpperBodyPose(leanS, leanC, twist);
}

void CompositeBehaviourController::requestKneeBend(double kb) {
	controllers[activeBehaviour]->requestKneeBend(kb);
}

void CompositeBehaviourController::requestDuckFootedness(double df) {
	controllers[activeBehaviour]->requestDuckFootedness(df);
}

void CompositeBehaviourController::requestCoronalStepWidth(double corSW) {
	controllers[activeBehaviour]->requestCoronalStepWidth(corSW);
}

void CompositeBehaviourController::requestElbowBend(double leftBend, double rightBend) {
	controllers[activeBehaviour]->requestElbowBend(leftBend, rightBend);
}

void CompositeBehaviourController::setDesiredSwingFootLocation() {
	controllers[activeBehaviour]->setDesiredSwingFootLocation();
}

/**
	determine the estimate desired location of the swing foot, given the estimated position of the COM, and the phase
*/
Vector3d CompositeBehaviourController::computeSwingFootLocationEstimate(const CartWheel::Math::Point3d& comPos, double phase) {

	controllers[activeBehaviour]->computeSwingFootLocationEstimate(comPos, phase);
}

/**
	ask for a heading...
*/
void CompositeBehaviourController::requestHeading(double v) {

	for (unsigned int i = 0; i < controllers.size(); i++) {
		controllers[i]->requestHeading(v);
	}
	double leftTwist = 0;
	double rightTwist = 0;
	double leftAdduction = 0;
	double rightAdduction = 0;
	double leftSwing = 0;
	double rightSwing = 0;
/*
	setShoulderAngles(leftTwist, rightTwist, leftAdduction,
			rightAdduction, leftSwing, rightSwing);
*/
}

/**
	sets a bunch of parameters to some default initial value
*/
void CompositeBehaviourController::initializeDefaultParameters() {
	for (unsigned int i = 0; i < controllers.size(); i++) {
		controllers[i]->initializeDefaultParameters();
	}
}

void CompositeBehaviourController::switchToNextController() {
	phi = 0.0;

	SimpleStyleParameters params;

	int i = activeBehaviour + 1;

	params.ubSagittalLean = controllers[i]->getDesiredSagittalLean();
	params.ubCoronalLean = controllers[i]->getDesiredCoronalLean();
	params.ubTwist = controllers[i]->getDesiredUpperBodyTwist();
	params.velDSagittal = controllers[i]->getDesiredVelocitySagittal();
	params.velDCoronal = controllers[i]->getDesiredVelocityCoronal();
	params.kneeBend = controllers[i]->getDesiredKneeBend();
	params.duckFootedness = controllers[i]->getDesiredDuckFootness();
	params.elbowBend = controllers[i]->getDesiredElbowBend();
	params.coronalStepWidth = controllers[i]->getCoronalStepWidth();
	params.elbowBend = controllers[i]->getDesiredElbowBend();
	params.shoulderTwist = controllers[i]->getDesiredShoulderTwist();
	params.shoulderCoronal = controllers[i]->getDesiredShoulderCoronal();
	params.shoulderSagittal = controllers[i]->getDesiredShoulderSagittal();

	params.stepHeight = controllers[i]->getDesiredStepHeight();

	params.applyInterpolatedStyleParameters(controllers[activeBehaviour], 0.5, &params);

	activeBehaviour++;
}

void CompositeBehaviourController::simStepPlan(double dt) {

	bool switchControllers = false;

	if (behaviourDurations.size() > 0) {
		double phase = dt/behaviourDurations[activeBehaviour];

		// Advance the phase of the controller
		phi += phase;

		if (phi > phase) {
			switchToNextController();
		}
	}

	controllers[activeBehaviour]->simStepPlan(dt);
}

/**
	this method gets called every time the controller transitions to a new state
*/
void CompositeBehaviourController::conTransitionPlan() {
	controllers[activeBehaviour]->conTransitionPlan();
}

/**
 Return the maximum panic level over the entire collection of controllers.
 */
double CompositeBehaviourController::getPanicLevel() {
	double maxPanicLevel = -1.0;

	for (unsigned int i = 0; i < controllers.size(); i++) {
		maxPanicLevel = max(maxPanicLevel, controllers[i]->getPanicLevel());
	}

	return maxPanicLevel;
}
