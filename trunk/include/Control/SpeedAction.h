#ifndef _SPEEDACTION_H_
#define _SPEEDACTION_H_

#include "TomsAction.h"

class SpeedAction : TomsAction{

public:
SpeedAction(){mySpeed = 0.0;}
SpeedAction(double s){mySpeed = s;};
void execute(CartWheel3D * cw);
void setSpeed(double d){mySpeed = d;};

private:
  double mySpeed;

};

#endif
