#ifndef _VFSMPPOS_H_
#define _VFSMPPOS_H_

#include <map>
#include <vector>
#include <Control/FsmTrans.h>
#include <Control/RelationalState.h>

namespace CartWheel
{
using namespace std;

class VfsmPos{

public:
VfsmPos(int n, map<int, vector<FsmTrans*>* >* tr, int s, map<int,bool>* t);
~VfsmPos();
void progress(RelationalState & r);
int getCurrent(){return current;}
bool getCurTerminal();
void reset(){current = start; lastTrans = NULL;};

protected:
int numStates;
map<int, vector<FsmTrans*>* >* trans;
int start;
map<int,bool>* terminals;
int current;
FsmTrans * lastTrans;

};


} // namespace CartWheel
#endif
