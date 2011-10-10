#include <Core/CompositeController.h>
#include <Core/ConUtils.h>
#include <Core/SimBiController.h>
#include <Core/IKVMCController.h>
#include <Core/WorldOracle.h>
#include <Core/TurnController.h>
#include <Core/CompositeBehaviourController.h>

using namespace CartWheel;
using namespace CartWheel::Core;
using namespace CartWheel::Physics;
using namespace CartWheel::Math;
using namespace CartWheel::Util;

CompositeController::CompositeController(Character* ch, WorldOracle* oracle, const char* input) : Controller(ch) {

    synchronizeControllers = false;
    //create space for the torques we will be using...
    for (int i = 0; i < jointCount; i++)
        torques.push_back(Vector3d());

    //now we'll interpret the input file...
    if (input == NULL)
        throwError("NULL file name provided.");
    FILE *f = fopen(input, "r");
    if (f == NULL)
        throwError("Could not open file: %s", input);

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
            case LOAD_CON_FILE:
            {
                //add a new controller
                SimBiController* controller = new SimBiController(character);
                controller->loadFromFile(trim(line));
                controllers.push_back(controller);
                break;
            }
            case LOAD_IKVMC_CON_FILE:
            {
                //add a new controller
                IKVMCController* controller = new IKVMCController(character);
                controller->loadFromFile(trim(line));
                //	BehaviourController* behaviour = new TurnController(character, controller, oracle);
                //	controller->setBehaviour(behaviour);
                //	behaviour->conTransitionPlan();
                controllers.push_back(controller);
                break;
            }
            case LOAD_TURN_BEHAVIOUR_CON_FILE:
            {
                // Assign to the associated controller. Normally it is the very last one loaded
                IKVMCController* controller = dynamic_cast<IKVMCController*> (controllers[controllers.size() - 1]);

                if (NULL != controller) {
                    // Initialize a new behaviour controller
                    BehaviourController* behaviour = new CompositeBehaviourController(ch, controller, oracle);
                    FILE* fileCBC = fopen(trim(line), "r");
                    if (fileCBC == NULL)
                        throwError("Could not open file: %s", line);
//                    printf("Reading File: %s\n", line);
                    behaviour->loadFromFile(fileCBC);
//                    printf("----------------\n");
                    fclose(fileCBC);

                    controller->setBehaviour(behaviour);
                    behaviour->conTransitionPlan();
                }
                break;
            }
            case CON_NOT_IMPORTANT:
                printf("Ignoring input line: \'%s\'\n", line);
                break;
            case CON_COMMENT:
                break;
            case CON_CHARACTER_STATE:
                character->loadReducedStateFromFile(trim(line));
                break;
            case CON_SYNCHRONIZE_CONTROLLERS:
                synchronizeControllers = true;
                break;
            case CON_STARTING_STANCE:
                //need to set the starting stance for all the controllers
                if (strncmp(trim(line), "left", 4) == 0) {
                    setStance(LEFT_STANCE);
                } else if (strncmp(trim(line), "right", 5) == 0) {
                    setStance(RIGHT_STANCE);
                } else
                    throwError("When using the \'reverseTargetOnStance\' keyword, \'left\' or \'right\' must be specified!");
                break;
            default:
                throwError("Incorrect SIMBICON input file: \'%s\' - unexpected line.", buffer);
        }
    }
    fclose(f);

    primaryControllerIndex = -1;
    secondaryControllerIndex = -1;
}

CompositeController::~CompositeController(void) {
}

/**
        This method is used to set the stance 
 */
void CompositeController::setStance(int newStance) {
    for (unsigned int i = 0; i < controllers.size(); i++)
        controllers[i]->setStance(newStance);
}

/**
        This method is used to compute the torques that are to be applied at the next step.
 */
void CompositeController::computeTorques(DynamicArray<ContactPoint> *cfs) {
    int cCount = (int) controllers.size();
    SimBiController* secondaryController = (secondaryControllerIndex < 0 || secondaryControllerIndex >= cCount) ? (NULL) : controllers[secondaryControllerIndex];
    SimBiController* primaryController = (primaryControllerIndex < 0 || primaryControllerIndex >= cCount) ? (NULL) : controllers[primaryControllerIndex];

    //we need the primary controller to be valid
    if (primaryController == NULL)
        throwError("A primary controller needs to always be selected for a composite controller!");

    //but it shouldn't be a big deal if the secondary one is not chosen
    if (secondaryController == NULL || synchronizeControllers == false) {
        //if we don't synchronize the controllers, then we won't interpolate either, since it doesn't make sense to interpolate between
        //controllers that, for instance, do not agree on which part of the walk cycle the character is at!!!
        primaryController->computeTorques(cfs);
        for (int i = 0; i < jointCount; i++)
            this->torques[i] = primaryController->torques[i];
        return;
    }

    if (interpValue > 1)
        interpValue = 1;

    if (interpValue < 0)
        interpValue = 0;

    //now compute the torques for the primary and secondary controllers
    primaryController->computeTorques(cfs);
    secondaryController->computeTorques(cfs);

    Vector3d tmp1;
    Vector3d tmp2;

    //and now interpolate between the torques of the two controllers...
    for (int i = 0; i < jointCount; i++) {
        tmp1 = primaryController->torques[i];
        tmp1.multiplyBy(interpValue);
        tmp2 = secondaryController->torques[i];
        tmp2.multiplyBy(1 - interpValue);
        this->torques[i].setToSum(tmp1, tmp2);
    }

    //and that's it
}

/**
        This method is used to advance the controller in time. It takes in a list of the contact points, since they might be
        used to determine when to transition to a new state. This method returns -1 if the controller does not advance to a new state,
        or the index of the state that it transitions to otherwise.
 */
int CompositeController::advanceInTime(double dt, DynamicArray<ContactPoint> *cfs) {
    int cCount = (int) controllers.size();
    SimBiController* secondaryController = (secondaryControllerIndex < 0 || secondaryControllerIndex >= cCount) ? (NULL) : controllers[secondaryControllerIndex];
    SimBiController* primaryController = (primaryControllerIndex < 0 || primaryControllerIndex >= cCount) ? (NULL) : controllers[primaryControllerIndex];

    // TODO: resolve controller interpolation
    primaryController->performPreTasks(dt, cfs);

    primaryController->performPostTasks(dt, cfs);

    return 0;
    //we need the primary controller to be valid
    if (primaryController == NULL)
        throwError("A primary controller needs to always be selected for a composite controller!");

    //we to advance in time the main controller. However, we need to know how much the phase should advance since we may be
    //interpolating with the secondary controller

    double dtOverT = dt / primaryController->states[primaryController->FSMStateIndex]->stateTime;

    if (secondaryController != NULL && synchronizeControllers == true)
        dtOverT = dt / ((interpValue) * primaryController->states[primaryController->FSMStateIndex]->stateTime + (1 - interpValue) * secondaryController->states[secondaryController->FSMStateIndex]->stateTime);


    //If the primary decides that it should switch to the next state, then we will force all the other controllers switch to the next state as well. 
    //Otherwise, we will just update their phi's so that they are all in phase

    int stateIndex;
    //advance in time the main controller, and see if it decides it should switch stance/FSM state
    bool newFSMState = ((stateIndex = primaryController->advanceInTime(primaryController->states[primaryController->FSMStateIndex]->stateTime * dtOverT, cfs)) != -1);

    //now take care of all the other controllers
    for (unsigned int i = 0; i < controllers.size(); i++) {
        controllers[i]->updateDAndV();
        //make sure we skip the controller that we just advanced in time above
        if (controllers[i] == primaryController)
            continue;

        //even if we're not synchronizing the FSMState/phi of the controllers, we will still make sure that the stance/groundContact is the same
        //for all the controllers
        controllers[i]->bodyTouchedTheGround = primaryController->isBodyInContactWithTheGround();
        if (synchronizeControllers == false) {
            controllers[i]->setStance(primaryController->stance);
            continue;
        }

        //if we are synchronizing the controllers, then we have to make sure they all transition at the same time, and that they all have the same phase
        if (newFSMState)
            controllers[i]->transitionToState(controllers[i]->states[controllers[i]->FSMStateIndex]->nextStateIndex);
        else
            controllers[i]->phi = primaryController->phi;
    }
    return stateIndex;
}

/**
        This method is used to populate the structure that is passed in with the current state
        of the controller;
 */
void CompositeController::getControllerState(CompositeControllerState* cs) {
    cs->interpValue = this->interpValue;
    cs->primaryControllerIndex = this->primaryControllerIndex;
    cs->secondaryControllerIndex = this->secondaryControllerIndex;
    cs->synchronizeControllers = this->synchronizeControllers;

    for (unsigned int i = 0; i < controllers.size(); i++) {
        SimBiControllerState s;
        controllers[i]->getControllerState(&s);
        cs->controllerStates.push_back(s);
    }
}

/**
        This method is used to populate the state of the controller, using the information passed in the
        structure
 */
void CompositeController::setControllerState(const CompositeControllerState &cs) {
    this->interpValue = cs.interpValue;
    this->primaryControllerIndex = cs.primaryControllerIndex;
    this->secondaryControllerIndex = cs.secondaryControllerIndex;
    this->synchronizeControllers = cs.synchronizeControllers;

    for (unsigned int i = 0; i < controllers.size(); i++)
        controllers[i]->setControllerState(cs.controllerStates[i]);
}

