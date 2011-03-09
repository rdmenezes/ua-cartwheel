#ifndef _SPEEDACTION_H_
#define _SPEEDACTION_H_

#include <Control/ExtendedAction.h>

namespace CartWheel
{
class WalkAction : public ExtendedAction {
    typedef ExtendedAction Base;

public:
WalkAction(){mySpeed = 0.0;}
WalkAction(double s){mySpeed = s;};
WalkAction(double t, double s) {myTime = t; mySpeed = s;};

void executeSetup(CartWheel3D * cw);
void setSpeed(double d){mySpeed = d;};

virtual void setParams(std::vector<double> & params);
virtual double getPrior(std::vector<double> & params);

virtual size_t numParams() const
{
    return Base::numParams() + myNumParams();
}

virtual double getParam(size_t i) const;
virtual double& getParam(size_t i);
virtual UnitType getParamUnits(size_t i) const;


private:
    size_t myNumParams() const 
    {
        return 1; // mySpeed;
    }

  double mySpeed;

};

} // namespace CartWheel
#endif
