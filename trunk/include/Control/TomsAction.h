#ifndef _TOMSACTION_H_
#define _TOMSACTION_H_

#include <Core/CartWheel3D.h>
#include<string>

using std::string;
using namespace CartWheel;
using namespace CartWheel::Core;

class TomsAction {

public:

TomsAction(){;};

virtual void setActor(string s){actorName =s;};

virtual string getActor(){return actorName;};

virtual void execute(CartWheel3D * cw) = 0;

protected:
 string actorName;

};


#endif
