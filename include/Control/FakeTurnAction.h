#ifndef _FAKETURNACTION_H_
#define _FAKETURNACTION_H_

#include "TomsAction.h"

class FakeTurnAction : public TomsAction {

public:
FakeTurnAction(){myChange = 0.0;}
FakeTurnAction(double change){myChange = change;}
void executeSetup(CartWheel3D * cw);

virtual void setParams(std::vector<double> params);

virtual double getPrior(std::vector<double> params);

private:
double myChange;
};

#endif
