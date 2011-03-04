#ifndef _FSMTRANS_H_
#define _FSMTRANS_H_

#include <Control/RelationalState.h>

class FsmTrans{

public:

FsmTrans(int s, int e, RelationalState *r);



bool checkFired(RelationalState & r);
int getEnd() const {return myEnd;};
int getStart() const{return myStart;};

protected:
int myStart;
int myEnd;
RelationalState *rs;  //need to add some subsume stuff to rs

};

#endif
