/*
 * StartState.h
 *
 *  Created on: Apr 5, 2011
 *      Author: dhewlett
 */

#ifndef STARTSTATE_H_
#define STARTSTATE_H_

#include <string>
#include <boost/shared_ptr.hpp>

namespace CartWheel {

class StartState;
typedef boost::shared_ptr<StartState> StartStatePtr;

class StartState
{
public:
  StartState(std::string name, double x, double z, double theta);
  virtual ~StartState();

  std::string getName() { return name_; };
  double getX() { return x_; };
  double getZ() { return z_; };
  double getTheta() { return theta_; };

  void setName(std::string name) { name_ = name; };
  void setX(double x) { x_ = x; };
  void setZ(double z) { z_ = z; };
  void setTheta(double theta) { theta_ = theta; };

private:
  std::string name_;
  double x_;
  double z_;
  double theta_;
};

}
#endif /* STARTSTATE_H_ */
