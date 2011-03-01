#include "Control/FakeTurnAction.h"


void FakeTurnAction::executeSetup(CartWheel3D * cw){
  //find the actor and set his speed
  int hIndex = atoi(actorName.c_str());  
  cw->setHumanHeading(hIndex, myChange + cw->getHumanHeading(hIndex));

}

void FakeTurnAction::setParams(std::vector<double> params) {
  setTime(params[0]);
  myChange = params[1];
}

double FakeTurnAction::getPrior(std::vector<double> params) {
  double timeM = 8.0;
  double timeS = 5.0;

  double speedM = 1.0;
  double speedS = 1.0;

//  ControlUtils::gaussianPD(timeM, timeS, params[0]);
//  ControlUtils::gaussianPD(speedM, speedS, params[1]);
}
