#ifndef _WRAPPERACTION_H_
#define _WRAPPERACTION_H_

#include <Core/CartWheel3D.h>
#include <Control/PosState.h>
#include <string>
#include <vector>
#include <map>
#include <Control/ExtendedAction.h>
#include <Control/ControlUtils.h>

class WrapperAction : public ExtendedAction {

public:
WrapperAction(std::string  c, std::vector<double> & v) : myParams() {myTime = v[0]; myParams = v; switched = true; populateMap(); myCommand = actionMap[c];};
WrapperAction(std::string  c, double t) : myParams() { myTime = t; switched = true; populateMap(); myCommand = actionMap[c];};
WrapperAction(int c, std::vector<double> & v) : myParams() { myCommand = c; myTime = v[0]; myParams = v; switched = true; populateMap();};
WrapperAction(int c, double t) : myParams() {myCommand = c; myTime = t; switched = true; populateMap();};

virtual std::string getActionName(int x);
int getNumActions(){return actionMap.size();}


virtual void executeSetup(CartWheel3D * cw);

virtual void setParams(std::vector<double> & params){myParams = params; switched = true;};
virtual double getPrior(std::vector<double> & params);

private:
std::vector<double> myParams;
int myCommand;
bool switched;
void populateMap(); 
map<string, int> actionMap;

};

#endif
