#ifndef _TOMSACTION_H_
#define _TOMSACTION_H_

#include <Core/CartWheel3D.h>
#include <Control/PosState.h>
#include <string>
#include <iostream>
#include <vector>
#include <stdexcept>

namespace CartWheel
{

using std::cout;
using std::endl;
using std::string;
using std::out_of_range;

using namespace CartWheel::Core;

enum UnitType { UNKNOWN_UNIT,  // error/bug
                  SPACIAL_UNIT,  // position, height, width, etc.
                  VSPACIAL_UNIT, // velocity
                  ASPACIAL_UNIT, // acceleration
                  ANGLE_UNIT,    // angles and orientations
                  VANGLE_UNIT,   // angular velocity
                  AANGLE_UNIT,   // angular acceleration
                  TIME_UNIT,     // seconds
                  OTHER_UNIT};    // placeholder (how to handle oddballs?)

class ExtendedAction {

public:

ExtendedAction() :
	actorName("Human1"),
	myTime(0.0) {}

virtual string getActor(){return actorName;};
virtual void setActor(string s){actorName =s;};

virtual double getTime() { return myTime; }
virtual void setTime(double l){myTime = l;}

virtual double getPrior(std::vector<double> & params) = 0;

virtual void setParams(std::vector<double> & params) = 0;

// the following three allow ExtendedAction to look like a "vector", and
// are implemented to ensure subclasses don't hijack parameters of parent classes
virtual size_t numParams() const
{
    return myNumParams();
}

virtual double getParam(size_t i) const
{
    if(i > myNumParams())
    {
        throw std::out_of_range("ExtendedAction::getParam -- index is out of range");
    }

    return myTime;
}

virtual double& getParam(size_t i)
{
    if(i > myNumParams())
    {
        throw std::out_of_range("ExtendedAction::getParam -- index is out of range");
    }

    return myTime;
}

virtual UnitType getParamUnits(size_t i) const
{
    if(i > myNumParams())
    {
        throw std::out_of_range("ExtendedAction::getParamUnits -- index is out of range");
    }

    return TIME_UNIT; //myTime
}

virtual void execute(CartWheel3D *cw);
virtual void executeStep(CartWheel3D *cw, double step);

virtual void executeSetup(CartWheel3D * cw) = 0;


protected:

 string actorName;
 double myTime;

private:
 size_t myNumParams() const { return 1; }
};

} // namespace CartWheel


#endif
