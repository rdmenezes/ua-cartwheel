#include <Control/WrapperAction.h>

namespace CartWheel
{

void WrapperAction::executeSetup(CartWheel3D * cw){
//  if(switched){
    int com = myCommand;
    if(myCommand == 10 || myCommand == 11)
	com = 0;
    cw->setController(actorName, com);  //TODO: how do we send parameters?
    if(myParams.size() > 1 && (myCommand == 1 || myCommand ==10))
	cw->setHumanSpeed(actorName, myParams[1]);
    else if(myParams.size() > 1)
	cw->setHumanHeading(actorName, myParams[1]);

    if(myParams.size() > 2 && myCommand == 10)
	cw->setHumanHeading(actorName, myParams[2]);
//    switched = false;
//  }

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
  actionMap["walkTurn"]=10; 
  actionMap["stillTurn"]=11;
}


double WrapperAction::getPrior(std::vector<double> & params) {
  double timeM = 8.0;
  double timeS = 5.0;

  double timePD = ControlUtils::gaussianPD(timeM, timeS, params[0]);

  return timePD;
}

double& WrapperAction::getParam(size_t i)
{
    if(i < Base::numParams())
        return Base::getParam(i);
    i -= Base::numParams();

    if(i >= myParams.size())
        throw std::out_of_range("WrapperAction::getParam -- index is out of range");

    return myParams[i];
}

double WrapperAction::getParam(size_t i) const
{
    WrapperAction& self = const_cast<WrapperAction&>(*this);
    return self.getParam(i);
}

} // namespace CartWheel
