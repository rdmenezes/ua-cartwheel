#ifndef _POSSTATE_H_
#define _POSSTATE_H_

#include <vector>
#include <string>
#include "Core/CartWheel3D.h"
#include <MathLib/Vector3d.h>


using CartWheel::Math::Vector3d;
using std::string;
using std::vector;
using namespace CartWheel;
using namespace CartWheel::Core;
using namespace CartWheel::Math;


class PosState{

public:
PosState():myPositions(),myNames(){};
PosState(CartWheel3D * cw);
const Vector3d* getPosition(string n);
void reset(CartWheel3D * cw);
int getNumVectors(){return myNames.size();};

protected:
  vector<Vector3d> myPositions;
  vector<string> myNames;
  void populate(CartWheel3D * cw);
};


#endif


