#include <Control/FsmTrans.h>


namespace CartWheel
{

FsmTrans::FsmTrans(int s, int e, RelationalState *r){
  myStart = s;
  myEnd = e;
  rs = r;
}

FsmTrans::~FsmTrans(){
	delete rs;
}

bool FsmTrans::checkFired(RelationalState & r){
	return r.containsAll(*rs);
}

} // namespace CartWheel
