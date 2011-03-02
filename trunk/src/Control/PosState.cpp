#include "Control/PosState.h"
#include <iostream>

using std::cout;
using std::endl;

PosState::PosState(CartWheel3D * cw):blacklist()
{
  blacklist["ground"] = 1;  
  int h = cw->getHumanCount();
   blacklist["pelvis"] = 1;
   for(int i =0; i < h; i++){
      stringstream ss;
      ss <<"pelvis"<<(i+1);
      blacklist[ss.str()] = 1; //TODO: Hacked this, not sure why the loop below doesn't fix it
      for(int j=0; j< cw->getHuman(i)->getCharacter()->getArticulatedRigidBodyCount(); j++){
          blacklist[string(cw->getHuman(i)->getCharacter()->getArticulatedRigidBody(j)->getName())] = 1;
	}
   }
   populate(cw);
}



void PosState::populate(CartWheel3D * cw)
{
  for (int i = 0; i < cw->getHumanCount(); i++)
  {
    myNames.push_back(cw->getHuman(i)->getName());
    myPositions.push_back(cw->getHumanPosition(i));
  }

  int others = cw->getWorld()->getRBCount();
  for(int j =0; j < others; j++){
     if(blacklist.count(cw->getWorld()->getRB(j)->getName()) == 0){
       myNames.push_back(cw->getWorld()->getRB(j)->getName());
       Point3d pp = cw->getWorld()->getRB(j)->getCMPosition();
       myPositions.push_back(*(new Vector3d(pp.getX(), pp.getY(), pp.getZ())));
     }
  }
}

void PosState::reset(CartWheel3D * cw)
{
  myNames.clear();
  myPositions.clear();
  populate(cw);
}

const Vector3d * PosState::getPosition(string n)
{
  for (int i = 0; i < myNames.size(); i++)
  {
    if (n.compare(myNames[i]) == 0)
      return &(myPositions[i]);
  }
  return NULL;
}
