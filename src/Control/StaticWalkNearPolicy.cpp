
#include "Control/StaticWalkNearPolicy.h"

double eucDistance(Vector3d &, Vector3d &);
double eucDistance2d(Vector3d &, Vector3d &);


StaticWalkNearPolicy::StaticWalkNearPolicy(double walkSpeed, string p){
   myWalkSpeed = walkSpeed;
   myAvailableActions = new TomsAction*[2];
   SpeedAction * temp = new SpeedAction(myWalkSpeed);
   myAvailableActions[0] = (TomsAction*) temp;
   temp = new SpeedAction(0.0);
   myAvailableActions[1] = (TomsAction*)temp;
   myControllables =new string[1];
   myControllables[0] = p;

} 

StaticWalkNearPolicy::~StaticWalkNearPolicy(){
	delete myAvailableActions[0];
	delete myAvailableActions[1];
	delete [] myAvailableActions;
        delete [] myControllables;
}


double eucDistance(Vector3d & a, Vector3d & b){
 	return sqrt((a -b).dotProductWith((a - b))); 
}

double eucDistance2d(Vector3d & a, Vector3d & b){
 	return sqrt(( a.getX() - b.getX()) * ( a.getX() - b.getX()) + ( a.getZ() - b.getZ()) * ( a.getZ() - b.getZ())  ); 
}


//TODO: We will be replacing the nearness check with a relation class once I verify this works
TomsAction* StaticWalkNearPolicy::getAction(CartWheel3D * simState){
  
  //find location of the controllable actor
	Vector3d pos1 = simState->getHumanPosition(atoi(myControllables[0].c_str()));
	 
	//cout<<"Made the first point"<<endl;
  //target location (later might be another actor)
       //cout<<"pos: "<<simState->getObjectByName("dodgeBox")->getCMPosition().getX()<<endl;
       Point3d pos21(simState->getObjectByName("dodgeBox")->getCMPosition());
       Vector3d pos2(pos21.getX(), pos21.getY(), pos21.getZ());	
       //cout<<"Made the points"<<endl;

  //keep walking
  cout<<"Box : "<<pos2.getX()<<"  "<<pos2.getY()<<"  "<<pos2.getZ()<<endl;
  cout<<"Distance : "<<eucDistance2d(pos1, pos2)<<endl;
  if(eucDistance2d(pos1, pos2) > 1.0){
  	cout<<"Walking"<<endl;	
  	return myAvailableActions[0];
  }
  //stop walking
  else{
	cout<<"Stopping"<<endl;
  	return myAvailableActions[1];
  }
}

