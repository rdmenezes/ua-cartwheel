#ifndef _FAKETURNACTION_H_
#define _FAKETURNACTION_H_

#include <Control/ExtendedAction.h>

class TurnAction : public ExtendedAction {

public:
TurnAction(){myChange = 0.0;}
TurnAction(double change){myChange = change;}
void executeSetup(CartWheel3D * cw);

virtual void setParams(std::vector<double> params);

virtual double getPrior(std::vector<double> params);

private:
double myChange;
};

#endif
