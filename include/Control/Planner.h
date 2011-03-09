#ifndef _PLANNER_H_
#define _PLANNER_H_

#include <Core/CartWheel3D.h>
#include <Control/ExtendedAction.h>
#include <string>

namespace CartWheel
{

using namespace CartWheel::Core;
using std::string;


class Planner {

public:

  Planner(){myAvailableActions = NULL; myControllables = NULL; }

  Planner(ExtendedAction** aa, string* c){
	myAvailableActions = aa;
        myControllables = c;
 }

  virtual ExtendedAction* getAction(CartWheel3D * simState) = 0;

protected:
  ExtendedAction** myAvailableActions; //for now assuming a discrete action set
  string* myControllables;
};

} // namespace CartWheel

#endif


