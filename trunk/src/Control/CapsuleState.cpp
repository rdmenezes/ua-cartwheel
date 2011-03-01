/*
 * CapsuleState.cpp
 *
 *  Created on: Mar 1, 2011
 *      Author: dhewlett
 */

#include <Control/CapsuleState.h>
#include <Core/Human.h>

using CartWheel::CartWheel3D;
using CartWheel::Core::Human;

CapsuleState::CapsuleState()
{
}

CapsuleState::CapsuleState(CartWheel3D* cw)
{
  populate(cw);
}

CapsuleState::~CapsuleState()
{
}

void CapsuleState::populate(CartWheel3D* cw)
{
  for (int i = 0; i < cw->getHumanCount(); ++i)
  {
    // TODO Collect the capsules.
    Human* human = cw->getHuman(i);

    names_.push_back(human->getName());
//    human->getCharacter()->getArticulatedRigidBodyCount();
  }
}
