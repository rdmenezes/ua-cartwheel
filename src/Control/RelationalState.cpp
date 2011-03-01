#include "Control/RelationalState.h"

RelationalState::RelationalState(vector<Relation> & rs):myRelations(rs){
}

bool RelationalState::contains(Relation & r){
  for(int x = 0; x < myRelations.size(); x++){
     if(r.equals(myRelations[x]))
	return true;
  }
  return false;

}

Vector3d RelationalState::findPlace(int x, CartWheel3D * cw){
   //if(x < cw->getHumanCount()){
     return cw->getHumanPosition(x);
   //}
   //else{
    // Point3d pp = cw->getWorld()->getRB(x - cw->getHumanCount())->getCMPosition();
      //  return Vector3d(pp.getX(), pp.getY(), pp.getZ());
   //}
}

string RelationalState::findName(int x, CartWheel3D * cw){
  //if(x < cw->getHumanCount()){
     return cw->getHuman(x)->getName();
   //}
   //else{
     //   return cw->getWorld()->getRB(x- cw->getHumanCount())->getName();
   //}

}


string RelationalState::toString(){
  string s = "[";
  for(int j =0; j < myRelations.size(); j++){
    s.append(myRelations[j].toString());
    if(j < myRelations.size() -1)
      s.append(",");
  }
  s.append("]");
  return s;
}

void RelationalState::reset(PosState & last, CartWheel3D * cw){
  double atThresh = 1.0;
  double changeThresh = 0.01;

  //get each human (just doing humans now
  myRelations.clear();
   if(last.getNumVectors() == 0)
     return; 
  int numThings = last.getNumVectors();
 //just binary relations for right now
  for(int i = 0; i < numThings; i++){
     Vector3d pos1 = findPlace(i, cw);
     string n1 = findName(i, cw);
     for(int j =0; j < numThings; j++){
	if(i ==j)
	   continue; 
        Vector3d pos2 = findPlace(j, cw);
	double dist = ControlUtils::eucDistance2d(pos1, pos2);
	string n2 = findName(j, cw);
	double prevDist = ControlUtils::eucDistance2d(*(last.getPosition(n1)), *(last.getPosition(n2)));
	
	cout<<"rel check "<<dist<<"  "<<prevDist<<endl; 
        if(dist < atThresh){
		addRelation(*(new Relation("At",n1,n2)));
	}
      
        if(fabs(dist - prevDist) > changeThresh && dist - prevDist < 0 ){
           addRelation(*(new Relation("DistanceDecreasing",n1,n2))); 
	}
	else if(fabs(dist - prevDist) > changeThresh && dist - prevDist > 0){
	   addRelation(*(new Relation("DistanceIncreasing",n1,n2)));
	}
	else{	
	   addRelation(*(new Relation("DistanceConstant", n1, n2)));
	}
     
	}
  }
}
