/*
 * BoxStartState.h
 *
 *  Created on: Apr 7, 2011
 *      Author: dhewlett
 */

#ifndef BOXSTARTSTATE_H_
#define BOXSTARTSTATE_H_

#include <boost/shared_ptr.hpp>
#include <Core/CartWheel3D.h>

namespace CartWheel
{

class BoxStartState;
typedef boost::shared_ptr<BoxStartState> BoxStartStatePtr;

class BoxStartState
{

public:
  BoxStartState(std::string name, Math::Vector3d position, double rotation, Math::Vector3d size, double mass) :
    name_(name), position_(position), rotation_(rotation), size_(size), mass_(mass)
  {
  };
  virtual ~BoxStartState();

  std::string getName()
  {
    return name_;
  };

  Math::Vector3d getPosition()
  {
    return position_;
  };

  double getRotation()
  {
    return rotation_;
  };


  Math::Vector3d getSize()
  {
    return size_;
  };

  double getMass()
  {
    return mass_;
  };

  void addToWorld(CartWheel3D* simulator)
  {
    simulator->addBox(name_, size_, position_, rotation_, mass_);
//    Quaternion orientation;
//    orientation.setToRotationQuaternion(rotation_, Vector3d(0, 1, 0));
//    Vector3d zeroVelocity;
//    simulator->updateRB(name_, position_, orientation, zeroVelocity);
  };

private:
  std::string name_;
  Math::Vector3d position_;
  double rotation_;
  Math::Vector3d size_;
  double mass_;

};

}
#endif /* BOXSTARTSTATE_H_ */

