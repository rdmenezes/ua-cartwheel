#include <Core/ActionCollectionPolicy.h>

using namespace CartWheel;
using namespace CartWheel::Core;
using namespace CartWheel::Physics;
using namespace CartWheel::Math;
using namespace CartWheel::Util;

ActionCollectionPolicy::ActionCollectionPolicy(CompositeController* con) : Policy(con){

	actionIndex = -1;
}

ActionCollectionPolicy::~ActionCollectionPolicy(void){
}


/**
	this method is used to select the primary and secondary controllers to be run based on the user input.
*/
void ActionCollectionPolicy::applyAction(){
	if (actionIndex >= 0){
		int pIn = actions[actionIndex].pIndex;
		int sIn = actions[actionIndex].sIndex;
		if (con->getStance() == RIGHT_STANCE){
			for (unsigned int i=0;i<conSwapList.size();i++){
				if (conSwapList[i].aIndex == pIn){
					pIn = conSwapList[i].bIndex;
					break;
				}
			}
			for (unsigned int i=0;i<conSwapList.size();i++){
				if (conSwapList[i].aIndex == sIn){
					sIn = conSwapList[i].bIndex;
					break;
				}
			}
		}
		con->setControllerInput(pIn, sIn, actions[actionIndex].t);
	}
}

/**
	this method is used to select the primary and secondary controllers to be run based on the user input.
*/
void ActionCollectionPolicy::applyActionNoSwap(){
	if (actionIndex >= 0){
		int pIn = actions[actionIndex].pIndex;
		int sIn = actions[actionIndex].sIndex;
		con->setControllerInput(pIn, sIn, actions[actionIndex].t);
	}
}

/**
	this method is used to read the action list from a file. The method returns the number of actions read.
*/
int ActionCollectionPolicy::loadActionsFromFile(const char* fName){
	FILE* f = fopen(fName, "r");

	if (f == NULL)
		throwError("Could not open file: %s", fName);

	char inputLine[200];
	bool conSwapMode = false;
	while (!feof(f)){
		if (readValidLine(inputLine, f)){
			//read off the action
			int p, s;
			double t;
			double w = 0;
			if (conSwapMode == false){
				if (sscanf(inputLine, "%d %d %lf %lf", &p, &s, &t, &w) >= 3){
					actions.push_back(CompositeAction(p, s, t, w));
					continue;
				}
			}
			if (strncmp(inputLine, "switchOnRightStance", strlen("switchOnRightStance")) == 0){
				conSwapMode = true;
				continue;
			}

			if (conSwapMode == true && sscanf(inputLine, "%d %d", &p, &s) == 2){
				conSwapList.push_back(ControllerSwap(p, s));
				continue;
			}
		}
	}
	fclose(f);
	return actions.size();
}

