#include<Control/VfsmPos.h>


VfsmPos::VfsmPos(int n, map<int, vector<FsmTrans*>* >* tr, int s, map<int,bool>* t){
	numStates = n;
	trans = tr;
	terminals = t;
	start = s;
      	current = s;
 	lastTrans = NULL;
}

VfsmPos::~VfsmPos(){
  for(int i =0; i < trans->size(); i++){
	for(int j =0; j < (*trans)[i]->size(); j++)
 		delete (*(*trans)[i])[j];  //kill off the transition links, they are the vfsm's responsibility
  }
  delete trans;
  delete terminals;
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


