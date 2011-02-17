
#include <Core/CartWheel3D.h>



#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <algorithm>


#include "Control/StaticWalkNearPolicy.h"
#include "Control/TomsAction.h"


//#define DEBUG_OUTPUT

using std::max;
using std::min;

using std::vector;
using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::ifstream;


using namespace CartWheel;
using namespace CartWheel::Core;
using namespace CartWheel::Math;


double angle;
double speed;



CartWheel3D* simulator;
char* sPath;


static int process_option(const char* option, const char* value);


//const double SQRT2 = sqrt(2);
const double SQRT2 = M_SQRT2; 


		
void init_simulation(int argc, char** argv)
{
    sPath = new char[200];
    strcpy(sPath, "/data/PSI/");
    simulator = new CartWheel3D(sPath);
    simulator->addObject("ground", "data/objects/flatGround.rbs", -1);

     simulator->addObject("dodgeBox", "data/objects/box.rbs", 1);
	

    string humanModel = "data/characters/bipV3.rbs";
    string humanController = "data/controllers/bipV3/Walking.sbc";

     simulator->addHuman(humanModel, humanController, 
     Point3d(0.0, 1.0, -1.0), 0);
     //simulator->setHumanSpeed(0, speed);

//	cout<<"here1"<<endl;
    // TODO: validate the actual image size
//    visualization = new Visualization(render, argc, argv, 960, 540);
 //   visualization->setCameraLocation(Point3d(-3.9,1.1,0));
   // visualization->setCameraTarget(Point3d(0,1.1,0));
//	cout<<"here2"<<endl;
}


int main(int argc, char** argv)
{
 
  
  StaticWalkNearPolicy pol(1.2, "0"); 

  FILE* option_fp;

  init_simulation(argc, argv);
  cout<<"HERE"<<endl;
  for(int timer = 0; timer < 10000; timer++){
  	pol.getAction(simulator)->execute(simulator);
	simulator->runStep(.001);   	
	cout<<"human location : "<<simulator->getHumanPosition(0).getX()<<"  "<<simulator->getHumanPosition(0).getZ()<<endl;
   }


}
