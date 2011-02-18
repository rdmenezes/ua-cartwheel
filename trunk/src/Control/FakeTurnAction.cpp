#include "Control/FakeTurnAction.h"


void FakeTurnAction::execute(CartWheel3D * cw){
  //find the actor and set his speed
  int hIndex = atoi(actorName.c_str());  
  cw->setHumanHeading(hIndex, myChange + cw->getHumanHeading(hIndex));

}

