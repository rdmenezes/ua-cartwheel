#ifndef _TOMSACTION_H_
#define _TOMSACTION_H_

#include <Core/CartWheel3D.h>
#include <Control/PosState.h>
#include <string>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using namespace CartWheel;
using namespace CartWheel::Core;

class ExtendedAction {

public:

ExtendedAction(){;};

virtual string getActor(){return actorName;};
virtual void setActor(string s){actorName =s;};

virtual double getTime() { return myTime; }
virtual void setTime(double l){myTime = l;}

virtual double getPrior(std::vector<double> params) = 0;

virtual void setParams(std::vector<double> params) = 0;

virtual void execute(CartWheel3D *cw, std::vector<PosState*> *trajectory);
void executeStep(CartWheel3D *cw, double step);

virtual void executeSetup(CartWheel3D * cw) = 0;

protected:
 string actorName;
 double myTime;
};


#endif
