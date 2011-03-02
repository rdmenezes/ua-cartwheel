#include "Control/PosState.h"
#include <iostream>
#include <list>

using std::cout;
using std::endl;
using std::list;
using std::string;

PosState::PosState()
{

}

PosState::PosState(CartWheel3D * cw):blacklist()
{
  Human* human = NULL;

  list<string> humanNames;
  bool result = cw->getHumanNames(humanNames);

  blacklist["pelvis"] = 1;

  list<string>::const_iterator itr = humanNames.begin();
  for (int i = 0; itr != humanNames.end(); itr++, i++)
  {
	  string name = (*itr);
	  cw->getHuman(name, &human);

	  stringstream ss;
	  ss <<"pelvis"<<(i+1);
	  blacklist[ss.str()] = 1; //TODO: Hacked this, not sure why the loop below doesn't fix it

	  for(int j=0; j < human->getCharacter()->getArticulatedRigidBodyCount(); j++){
		  blacklist[string(human->getCharacter()->getArticulatedRigidBody(j)->getName())] = 1;
	  }
   }
   populate(cw);
}

void PosState::populate(CartWheel3D * cw)
{
  list<string> humanNames;
  bool result = cw->getHumanNames(humanNames);

  list<string>::const_iterator itr = humanNames.begin();
  for (int i = 0; itr != humanNames.end(); itr++, i++)
  {
	string name = (*itr);
    myNames.push_back(name);
    myPositions.push_back(cw->getHumanPosition(name));
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
