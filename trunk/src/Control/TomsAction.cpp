#include "Control/TomsAction.h"



void TomsAction::execute(CartWheel3D * cw){
  //time_t starter; 
  executeSetup(cw);
  //time(&starter);
  //starter = time(NULL);
  //while(time(NULL) - starter -  myTime < 0){
  for(double x =0.0; x < myTime * 20.0; x++) { 
     cout << "human location : " << cw->getHumanPosition(0).getX() << "  "<<cw->getHumanPosition(0).getZ() << endl; 
     cw->runStep(.001);
   }
  //}
  

}
