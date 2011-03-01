#ifndef _FAKETURNACTION_H_
#define _FAKETURNACTION_H_

#include "TomsAction.h"

class FakeTurnAction : TomsAction{

public:
FakeTurnAction(){myChange = 0.0;}
FakeTurnAction(double change){myChange = change;}
void executeSetup(CartWheel3D * cw);


private:
double myChange;
};

#endif
