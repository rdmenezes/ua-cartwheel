#ifndef _PLANNER_H_
#define _PLANNER_H_

#include <Core/CartWheel3D.h>
#include "Control/TomsAction.h"
#include<string>

using namespace CartWheel;
using namespace CartWheel::Core;
using std::string;


class Planner {

public:

  Planner(){myAvailableActions = NULL; myControllables = NULL; }

  Planner(TomsAction** aa, string* c){
	myAvailableActions = aa;
        myControllables = c;
 }

  virtual TomsAction* getAction(CartWheel3D * simState) = 0;

protected:
  TomsAction** myAvailableActions; //for now assuming a discrete action set
  string* myControllables;
};


#endif

