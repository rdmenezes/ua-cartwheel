#ifndef _FSMTRANS_H_
#define _FSMTRANS_H_

#include <Control/RelationalState.h>

namespace CartWheel
{
class FsmTrans{

public:

FsmTrans(int s, int e, RelationalState *r);
~FsmTrans();


bool checkFired(RelationalState & r);
int getEnd() const {return myEnd;};
int getStart() const{return myStart;};

protected:
int myStart;
int myEnd;
RelationalState *rs;  //need to add some subsume stuff to rs

};


} // namespace CartWheel
#endif
