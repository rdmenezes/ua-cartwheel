#include "Control/SpeedAction.h"

void SpeedAction::execute(CartWheel3D * cw){
  //find the actor and set his speed
    cw->setHumanSpeed(atoi(actorName.c_str()), mySpeed);


}

