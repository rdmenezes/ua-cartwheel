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
  BoxStartState(std::string name, Math::Vector3d position, Math::Vector3d size, double mass) :
    name_(name), position_(position), size_(size), mass_(mass)
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
    simulator->addBox(name_, size_, position_, mass_);
  };

private:
  std::string name_;
  Math::Vector3d position_;
  Math::Vector3d size_;
  double mass_;

};

}
#endif /* BOXSTARTSTATE_H_ */

