#ifndef _RELATIONALSTATE_H_
#define _RELATIONALSTATE_H_

#include <string>
#include <vector>
#include <iostream>
#include "Control/ControlUtils.h"
#include "Control/Relation.h"
#include "Control/PosState.h"

using namespace std;

class RelationalState{

public:
RelationalState():myRelations(){};
RelationalState(vector<Relation> & rs);
~RelationalState();
vector<Relation*>* getRelations(){return &myRelations;}
bool contains(const Relation & r);
void addRelation(Relation & r){myRelations.push_back(&r);};
void reset(PosState & last, CartWheel3D * cw);
string toString();
bool containsAll(RelationalState &);

protected:
void fullClear();
vector<Relation*> myRelations;
Vector3d findPlace(int, PosState &, CartWheel3D *);
string findName(int, PosState & last);

};


#endif
