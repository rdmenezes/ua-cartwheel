#ifndef _TOMSACTION_H_
#define _TOMSACTION_H_

#include <Core/CartWheel3D.h>
#include<string>
#include <iostream>

using std::cout;
using std::endl;
using std::string;
using namespace CartWheel;
using namespace CartWheel::Core;

class TomsAction {

public:

TomsAction(){;};

virtual void setActor(string s){actorName =s;};

virtual string getActor(){return actorName;};

virtual void execute(CartWheel3D * cw);

virtual void setTime(double l){myTime = l;}

protected:
 virtual void executeSetup(CartWheel3D * cw) = 0;
 string actorName;
 double myTime;

};


#endif
