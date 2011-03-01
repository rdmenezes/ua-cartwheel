#ifndef _RELATIONALSTATE_H_
#define _RELATIONALSTATE_H_

#include <string>
#include <vector>
#include "Control/ControlUtils.h"
#include "Control/Relation.h"
#include "Control/PosState.h"

using namespace std;

class RelationalState{

public:
RelationalState():myRelations(){};
RelationalState(vector<Relation> & rs);
vector<Relation>* getRelations(){return &myRelations;}
bool contains(Relation & r);
void addRelation(Relation & r){myRelations.push_back(r);};
void reset(PosState & last, CartWheel3D * cw);
string toString();

protected:
vector<Relation> myRelations;
Vector3d findPlace(int, CartWheel3D *);
string findName(int, CartWheel3D *);

};


#endif
