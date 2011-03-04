#include <Control/FsmTrans.h>



FsmTrans::FsmTrans(int s, int e, RelationalState *r){
  myStart = s;
  myEnd = e;
  rs = r;
}

bool FsmTrans::checkFired(RelationalState & r){
	return r.containsAll(*rs);
}
