#ifndef _STATICWALKNEARPOLICY_H_
#define _STATICWALKNEARPOLICY_H_

#include "Control/Planner.h"
#include <string>
#include <MathLib/Vector3d.h>
#include <iostream>
#include "Control/WalkAction.h"
#include "Control/TurnAction.h"
#include <math.h>
#include "Control/ControlUtils.h"

namespace CartWheel
{
using CartWheel::Math::Vector3d;
using std::string;
using std::cout;
using std::endl;

class StaticWalkNearPolicy : public Planner {


public:
  StaticWalkNearPolicy(double walkSpeed, string p);
  ~StaticWalkNearPolicy();
  ExtendedAction* getAction(CartWheel3D * simState);
protected:
  double myWalkSpeed;  

};

}
#endif
