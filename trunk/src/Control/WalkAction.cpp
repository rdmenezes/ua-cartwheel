#include <Control/WalkAction.h>
#include <Control/ControlUtils.h>

void WalkAction::executeSetup(CartWheel3D * cw){
  //find the actor and set his speed
    cw->setHumanSpeed(actorName, mySpeed);
}

void WalkAction::setParams(std::vector<double> & params) {
  setTime(params[0]);
  setSpeed(params[1]);
}

double WalkAction::getPrior(std::vector<double> & params) {
  double timeM = 8.0;
  double timeS = 5.0;

  double speedM = 1.0;
  double speedS = 1.0;

  double timePD = ControlUtils::gaussianPD(timeM, timeS, params[0]);
  double speedPD = ControlUtils::gaussianPD(speedM, speedS, params[1]);

  return timePD * speedPD;
}

