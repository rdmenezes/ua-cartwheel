#include <Control/WrapperAction.h>


void WrapperAction::executeSetup(CartWheel3D * cw){
  if(switched){
    cw->setController(actorName, myCommand);  //TODO: how do we send parameters?
    if(myParams.size() > 1)
	cw->setHumanSpeed(actorName, myParams[1]);
    switched = false;
  }

}


std::string WrapperAction::getActionName(int x){
	std::map<std::string, int>::iterator iter;

        for (iter = actionMap.begin(); iter != actionMap.end(); iter++) {
           if(iter->second == x)
		return iter->first;
	}
	return "";
}

//this should be static, but being a bit lazy
void WrapperAction::populateMap(){
  actionMap["walk"] = 0;
  actionMap["standStill"]= 1;
  actionMap["jog"]= 2;
  actionMap["sit"]= 3;
  actionMap["standUp"]= 4;
  actionMap["push"]= 5;
  actionMap["pull"]= 6;
  actionMap["pickup"]= 7;
  actionMap["raiseArms"]= 8;
  actionMap["kick"]= 9;
  
}


double WrapperAction::getPrior(std::vector<double> & params) {
  double timeM = 8.0;
  double timeS = 5.0;

  double timePD = ControlUtils::gaussianPD(timeM, timeS, params[0]);

  return timePD;
}

