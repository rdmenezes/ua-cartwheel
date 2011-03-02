/*
 * CompositeBehaviourController.cpp
 *
 *  Created on: Feb 28, 2011
 *      Author: denis
 */

#include <Core/CompositeBehaviourController.h>
#include <Core/TurnController.h>

using namespace CartWheel;
using namespace CartWheel::Core;
using namespace CartWheel::Util;

CompositeBehaviourController::CompositeBehaviourController(Character* b, IKVMCController* llc, WorldOracle* w) :
		BehaviourController(b, llc, w) {
}

void CompositeBehaviourController::loadFromFile(FILE * f){
	if (f == NULL)
		throwError("Invalid file pointer.");
	//now we'll interpret the input file...

	//have a temporary buffer used to read the file line by line...
	char buffer[200];
	//this is where it happens.
	while (!feof(f)){
		//get a line from the file...
		fgets(buffer, 200, f);
		if (feof(f))
			break;
		if (strlen(buffer)>195)
			throwError("The input file contains a line that is longer than ~200 characters - not allowed");
		char *line = lTrim(buffer);
		int lineType = getConLineType(line);
		switch (lineType) {
			case LOAD_TURN_BEHAVIOUR_CON_FILE: {
				//add a new controller
				BehaviourController* behaviour = new TurnController(bip, lowLCon, wo);
				const char* input = trim(line);
				FILE *fTemp = fopen(input, "r");
				if (fTemp == NULL)
					throwError("Could not open file: %s", input);

				behaviour->loadFromFile(fTemp);

				fclose(fTemp);

				controllers.push_back(behaviour);
				break;
			}
			case CON_NOT_IMPORTANT:
				printf("Ignoring input line: \'%s\'\n", line);
				break;
			case CON_COMMENT:
				break;
			default:
				throwError("Incorrect SIMBICON input file: \'%s\' - unexpected line.", buffer);
		}
	}
	fclose(f);

}
