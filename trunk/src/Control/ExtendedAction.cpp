#include "Control/ExtendedAction.h"
#include <Control/PosState.h>

namespace CartWheel
{

void ExtendedAction::execute(CartWheel3D *cw)
{
  //time_t starter; 

  //time(&starter);
  //starter = time(NULL);
  //while(time(NULL) - starter -  myTime < 0){

  executeSetup(cw);

  for (double x = 0.0; x < myTime * 20.0; x++)
  {
    //cout << "human location : " << cw->getHumanPosition(0).getX() << "  " << cw->getHumanPosition(0).getZ() << endl;
    //PosState* pos = new PosState(cw);
    //trajectory->push_back(pos);
    cw->runStep(.001);
  }
}

void ExtendedAction::executeStep(CartWheel3D *cw, double step)
{
//  cout << "human location (0): " << cw->getHumanPosition(0).getX() << "  " << cw->getHumanPosition(0).getZ() << endl;
  cw->runStep(step);
}

} //namespace CartWheel
