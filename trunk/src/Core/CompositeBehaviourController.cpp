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

CompositeBehaviourController::CompositeBehaviourController(Character* b, IKVMCController* llc, WorldOracle* w) :
	BehaviourController(b, llc, w),
	m_behaviour(NULL),
	activeBehaviour (0),
	timeElapsed (0.0),
	timeTransitioned (0.0),
	loopBehaviours (false),
	processAllBehaviours(false) {
}

CompositeBehaviourController::~CompositeBehaviourController() {
	initialStateControllers.clear();
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
		case CON_TURN_BEHAVIOUR:
			addController(new TurnController(bip, lowLCon, wo), f);
			break;
		case CON_DUCK_BEHAVIOUR:
			addController(new DuckController(bip, lowLCon, wo), f);
			break;
		case CON_DURATION_TIME: {
			double duration = 0.0;
			if (sscanf(line, "%lf", &duration)!=1)
				throwError("Controller duration time is missing from the file.");
			behaviourDurations.push_back(duration);
			break;
		}
		case CON_TRANSITION_TIME: {
			double transition = 0.0;
			if (sscanf(line, "%lf", &transition)!=1)
				throwError("Controller transition time is missing from the file.");
			behaviourTransitions.push_back(transition);
			break;
		}
		case CON_NOT_IMPORTANT:
			printf("Ignoring input line: \'%s\'\n", line);
			break;
		case CON_COMMENT:
			break;
		default:
			throwError("Incorrect m_behaviour input file: \'%s\' - unexpected line.",	buffer);
		}
	}

	// Start with the very first behaviour
	m_behaviour = controllers[0];

	initialStateControllers.reserve(controllers.size());
	for (unsigned int i = 0; i < controllers.size(); i++) {
		//initialStateControllers.push_back(controllers[i]);
		initialStateControllers[i] = new BehaviourController(*controllers[i]);
	}

	// If there are more than one behaviour define, we will process them all
	// in the sequence define by their order in the respective file
	processAllBehaviours = (behaviourDurations.size() > 0);

	if (processAllBehaviours) {
		// If the very last entry is the duration, loop around indefinately
		loopBehaviours = controllers.size() == behaviourDurations.size();
	}
}

void CompositeBehaviourController::saveToFile(FILE * file) {
	// TODO: to be implemented
}

void CompositeBehaviourController::addController(BehaviourController* behaviour, FILE * file) {
	// Load from the file if the handle is provided
	if (NULL != file) {
		behaviour->loadFromFile(file);
	}

	// Each controller must have both the duration and transition
	// except if there is only one controller at all
	if (controllers.size() != behaviourDurations.size()) {
		throwError("Mismatch in the number of controllers and time durations.");
	}
	else if (controllers.size() != behaviourTransitions.size()) {
		throwError("Mismatch in the number of controllers and time durations.");
	}

	controllers.push_back(behaviour);
}

void CompositeBehaviourController::setUpperBodyPose(double leanSagittal, double leanCoronal, double twist) {
	m_behaviour->setUpperBodyPose(leanSagittal, leanCoronal, twist);
}

void CompositeBehaviourController::setKneeBend(double v, bool swingAlso) {
	m_behaviour->setKneeBend(v, swingAlso);
}

void CompositeBehaviourController::setDuckWalkDegree(double v) {
	m_behaviour->setDuckWalkDegree(v);
}

void CompositeBehaviourController::setDesiredHeading(double v) {
	for (unsigned int i = 0; i < controllers.size(); i++) {
		controllers[i]->setDesiredHeading(i);
	}
}

void CompositeBehaviourController::setVelocities(double velDS, double velDC) {
	m_behaviour->setVelocities(velDS, velDC);
}

void CompositeBehaviourController::adjustStepHeight() {
	m_behaviour->adjustStepHeight();
}

void CompositeBehaviourController::setElbowAngles(double leftElbowAngle, double rightElbowAngle) {
	m_behaviour->setElbowAngles(leftElbowAngle, rightElbowAngle);
}

void CompositeBehaviourController::setShoulderAngles(double leftTwist, double rightTwist, double leftAdduction,
		double rightAdduction, double leftSwing, double rightSwing) {
	m_behaviour->setShoulderAngles(leftTwist, rightTwist, leftAdduction, rightAdduction,
			leftSwing, rightSwing);
}

void CompositeBehaviourController::requestStepTime(double stepTime) {
	m_behaviour->requestStepTime(stepTime);
}

void CompositeBehaviourController::requestStepHeight(double stepHeight) {
	m_behaviour->requestStepHeight(stepHeight);
}

void CompositeBehaviourController::requestVelocities(double velDS, double velDC) {
	m_behaviour->requestVelocities(velDS, velDC);
}

void CompositeBehaviourController::requestUpperBodyPose(double leanS, double leanC, double twist) {
	m_behaviour->requestUpperBodyPose(leanS, leanC, twist);
}

void CompositeBehaviourController::requestKneeBend(double kb) {
	m_behaviour->requestKneeBend(kb);
}

void CompositeBehaviourController::requestDuckFootedness(double df) {
	m_behaviour->requestDuckFootedness(df);
}

void CompositeBehaviourController::requestCoronalStepWidth(double corSW) {
	m_behaviour->requestCoronalStepWidth(corSW);
}

void CompositeBehaviourController::requestElbowBend(double leftBend, double rightBend) {
	m_behaviour->requestElbowBend(leftBend, rightBend);
}

void CompositeBehaviourController::setDesiredSwingFootLocation() {
	m_behaviour->setDesiredSwingFootLocation();
}

/**
	determine the estimate desired location of the swing foot, given the estimated position of the COM, and the phase
*/
Vector3d CompositeBehaviourController::computeSwingFootLocationEstimate(const CartWheel::Math::Point3d& comPos, double phase) {
	m_behaviour->computeSwingFootLocationEstimate(comPos, phase);
}

/**
	ask for a heading...
*/
void CompositeBehaviourController::requestHeading(double v) {
	for (unsigned int i = 0; i < controllers.size(); i++) {
		controllers[i]->requestHeading(v);
	}
}

/**
	sets a bunch of parameters to some default initial value
*/
void CompositeBehaviourController::initializeDefaultParameters() {
	for (unsigned int i = 0; i < controllers.size(); i++) {
		controllers[i]->initializeDefaultParameters();
	}
}

/**
 * Switch to the next controller
 */
void CompositeBehaviourController::switchToNextController(double dt) {

	SimpleStyleParameters params;

	int i = 0;

	if ((activeBehaviour + 1) < controllers.size()) {
		i = activeBehaviour + 1;
	}
	else {
		i = 0;
	}

	double transition = behaviourTransitions[activeBehaviour];
	double phase = 0.0;

	if (transition > 0.0) {
		double delta = transition / dt;
		phase = timeTransitioned / transition;
	}

	params.ubSagittalLean = initialStateControllers[activeBehaviour]->getDesiredSagittalLean();
	params.ubCoronalLean = initialStateControllers[activeBehaviour]->getDesiredCoronalLean();
	params.ubTwist = initialStateControllers[activeBehaviour]->getDesiredUpperBodyTwist();
	params.velDSagittal = initialStateControllers[activeBehaviour]->getDesiredVelocitySagittal();
	params.velDCoronal = initialStateControllers[activeBehaviour]->getDesiredVelocityCoronal();
	params.kneeBend = initialStateControllers[activeBehaviour]->getDesiredKneeBend();
	params.duckFootedness = initialStateControllers[activeBehaviour]->getDesiredDuckFootness();
	params.elbowBend = initialStateControllers[activeBehaviour]->getDesiredElbowBend();
	params.coronalStepWidth = initialStateControllers[activeBehaviour]->getCoronalStepWidth();
	params.elbowBend = initialStateControllers[activeBehaviour]->getDesiredElbowBend();
	params.shoulderTwist = initialStateControllers[activeBehaviour]->getDesiredShoulderTwist();
	params.shoulderCoronal = initialStateControllers[activeBehaviour]->getDesiredShoulderCoronal();
	params.shoulderSagittal = initialStateControllers[activeBehaviour]->getDesiredShoulderSagittal();
	params.stepHeight = initialStateControllers[activeBehaviour]->getDesiredStepHeight();

	SimpleStyleParameters newParams;
	newParams.ubSagittalLean = initialStateControllers[i]->getDesiredSagittalLean();
	newParams.ubCoronalLean = initialStateControllers[i]->getDesiredCoronalLean();
	newParams.ubTwist = initialStateControllers[i]->getDesiredUpperBodyTwist();
	newParams.velDSagittal = initialStateControllers[i]->getDesiredVelocitySagittal();
	newParams.velDCoronal = initialStateControllers[i]->getDesiredVelocityCoronal();
	newParams.kneeBend = initialStateControllers[i]->getDesiredKneeBend();
	newParams.duckFootedness = initialStateControllers[i]->getDesiredDuckFootness();
	newParams.elbowBend = initialStateControllers[i]->getDesiredElbowBend();
	newParams.coronalStepWidth = initialStateControllers[i]->getCoronalStepWidth();
	newParams.elbowBend = initialStateControllers[i]->getDesiredElbowBend();
	newParams.shoulderTwist = initialStateControllers[i]->getDesiredShoulderTwist();
	newParams.shoulderCoronal = initialStateControllers[i]->getDesiredShoulderCoronal();
	newParams.shoulderSagittal = initialStateControllers[i]->getDesiredShoulderSagittal();
	newParams.stepHeight = initialStateControllers[i]->getDesiredStepHeight();

	newParams.applyInterpolatedStyleParameters(m_behaviour, phase, &params);

	timeTransitioned += dt;

	// If done transitioning, reset the time and switch the behaviour
	if (timeTransitioned >= transition) {
		timeTransitioned = 0.0;
		timeElapsed = 0.0;

		if ((activeBehaviour + 1) < controllers.size()) {
			activeBehaviour = activeBehaviour + 1;
		} else { // We are done processing all the behaviours
			processAllBehaviours = loopBehaviours;

			if (loopBehaviours) {
				activeBehaviour = 0;
			}
		}
	}
}

void CompositeBehaviourController::simStepPlan(double dt) {
	if (processAllBehaviours) {
		double duration = behaviourDurations[activeBehaviour];

		// Advance the phase of the controller
		timeElapsed += dt;

		if (timeElapsed > duration) {
			switchToNextController(dt);
		}
	}

	m_behaviour->simStepPlan(dt);
}

/**
	this method gets called every time the controller transitions to a new state
*/
void CompositeBehaviourController::conTransitionPlan() {
	m_behaviour->conTransitionPlan();
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