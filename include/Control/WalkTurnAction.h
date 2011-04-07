#ifndef _WALKTURNACTION_H_
#define _WALKTURNACTION_H_

#include <Control/ExtendedAction.h>
#include <Control/WalkAction.h>

namespace CartWheel
{
class WalkTurnAction : public WalkAction {
    typedef WalkAction Base;

public:
WalkTurnAction(){mySpeed = 0.0; myHeading = 0.0;}
WalkTurnAction(double t, double s, double h) {myTime = t; mySpeed = s; myHeading=h;};

virtual void executeSetup(CartWheel3D * cw);
virtual void setParams(std::vector<double> & params);

virtual double getParam(size_t i) const;
virtual double& getParam(size_t i);
virtual UnitType getParamUnits(size_t i) const;



void setHeading(double d){myHeading = d;};

private:
    size_t myNumParams() const 
    {
        return 1; // myHeading;
    }

  double myHeading; //in radians

};

} // namespace CartWheel
#endif
