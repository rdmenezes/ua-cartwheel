#ifndef _POSSTATE_H_
#define _POSSTATE_H_

#include <map>
#include <vector>
#include <string>
#include "Core/CartWheel3D.h"
#include <MathLib/Vector3d.h>
#include <sstream>


using CartWheel::Math::Vector3d;
using std::string;
using std::vector;
using std::map;
using std::stringstream;

using namespace CartWheel;
using namespace CartWheel::Core;
using namespace CartWheel::Math;

class PosState {

public:
PosState();
PosState(CartWheel3D * cw);
const Vector3d* getPosition(string n);
void reset(CartWheel3D * cw);
int getNumVectors(){return myNames.size();};

string getName(int index) { return myNames[index]; };
Vector3d getPosition(int index) { return myPositions[index]; };

protected:
  map<string, int> blacklist;
  vector<Vector3d> myPositions;
  vector<string> myNames;
  void populate(CartWheel3D * cw);
};

#endif
