#ifndef _WRAPPERACTION_H_
#define _WRAPPERACTION_H_

#include <string>
#include <vector>
#include <map>

#include <Core/CartWheel3D.h>
#include <Control/PosState.h>
#include <Control/ExtendedAction.h>
#include <Control/ControlUtils.h>

namespace CartWheel
{

class WrapperAction : public ExtendedAction
{
  typedef ExtendedAction Base;

public:

  // New
  WrapperAction(std::string action, std::string human, std::vector<double> const& params)
  {
    myTime = params[0];
    myParams = params;
    populateMap();
    myCommand = actionMap[action];
    setActor(human);
  }

  WrapperAction(std::string c, std::vector<double> & v) :
    myParams()
  {
    myTime = v[0];
    myParams = v;
    switched = true;
    populateMap();
    myCommand = actionMap[c];
  }

  WrapperAction(std::string c, double t) :
    myParams()
  {
    myTime = t;
    switched = true;
    populateMap();
    myCommand = actionMap[c];
  }

  WrapperAction(int c, std::vector<double> & v) :
    myParams()
  {
    myCommand = c;
    myTime = v[0];
    myParams = v;
    switched = true;
    populateMap();
  }

  WrapperAction(int c, double t) :
    myParams()
  {
    myCommand = c;
    myTime = t;
    switched = true;
    populateMap();
  }

  virtual std::string getActionName(int x);
  int getNumActions()
  {
    return actionMap.size();
  }

  virtual void executeSetup(CartWheel3D * cw);

  virtual void setParams(std::vector<double> & params)
  {
    myParams = params;
    switched = true;
  }

  virtual double getPrior(std::vector<double> & params);

  virtual size_t numParams()
  {
    return Base::numParams() + myParams.size();
  }

  virtual double& getParam(size_t i);

  virtual double getParam(size_t i) const;

  virtual UnitType getPAramUnits(size_t i) const
  {
    return UNKNOWN_UNIT;
  }

private:

  std::vector<double> myParams;
  int myCommand;
  bool switched;
  void populateMap();
  map<string, int> actionMap;

};

} // namespace CartWheel
#endif
