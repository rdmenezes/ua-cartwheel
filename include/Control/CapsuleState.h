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

  //  const Vector3d* getPosition(string n);
  //  void reset(CartWheel3D * cw);
  //  int getNumVectors(){return myNames.size();};
  //
  //  string getName(int index) { return myNames[index]; };
  //  Vector3d getPosition(int index) { return myPositions[index]; };
  //
protected:
  std::vector<std::string> names_;
  std::vector<CartWheel::Math::Capsule> capsules_;
  void populate(CartWheel3D* cw);

};

#endif /* CAPSULESTATE_H_ */
