/*
 * CapsuleState.h
 *
 *  Created on: Mar 1, 2011
 *      Author: dhewlett
 */

#ifndef CAPSULESTATE_H_
#define CAPSULESTATE_H_

#include <Core/CartWheel3D.h>
#include <vector>

using CartWheel::CartWheel3D;

class CapsuleState
{
public:
  CapsuleState();
  CapsuleState(CartWheel3D* cw);
  virtual ~CapsuleState();

  int getNumEntities();
  std::string getName(int index);
  std::vector<CartWheel::Math::Capsule*> getCapsules(int index);

  void clear();

protected:
  void populate(CartWheel3D* cw);

  std::vector<std::string> names_;
  std::vector<std::vector<CartWheel::Math::Capsule*> > capsules_;

};

#endif /* CAPSULESTATE_H_ */
