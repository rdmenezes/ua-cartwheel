#ifndef _SPEEDACTION_H_
#define _SPEEDACTION_H_

#include <Control/TomsAction.h>

class WalkAction : public TomsAction {

public:
WalkAction(){mySpeed = 0.0;}
WalkAction(double s){mySpeed = s;};
WalkAction(double t, double s) {myTime = t; mySpeed = s;};

void executeSetup(CartWheel3D * cw);
void setSpeed(double d){mySpeed = d;};

virtual void setParams(std::vector<double> params);
virtual double getPrior(std::vector<double> params);

private:
  double mySpeed;

};

#endif
