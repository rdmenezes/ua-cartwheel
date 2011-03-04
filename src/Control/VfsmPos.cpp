#include<Control/VfsmPos.h>


VfsmPos::VfsmPos(int n, map<int, vector<FsmTrans*>* >* tr, int s, map<int,bool>* t){
	numStates = n;
	trans = tr;
	terminals = t;
	start = s;
      	current = s;
 	lastTrans = NULL;
}
  
bool VfsmPos::getCurTerminal(){
  return(terminals->count(current)!=0);
}

void VfsmPos::progress(RelationalState & r){
	 vector<FsmTrans*> * possible = (*trans)[current];
	vector<FsmTrans*>::const_iterator itr = possible->begin();
  bool fired = false;
   for (; itr != possible->end(); itr++){
  	if((*itr)->checkFired(r)){ //TODO: tie-breaking
		current = (*itr)->getEnd();
		lastTrans = *itr;
		fired = true;
	}
   }
   if(!fired && lastTrans != NULL &&!lastTrans->checkFired(r)){
  	current = start;
	lastTrans = NULL;
   }
 	
}


