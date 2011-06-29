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
  StartState(int type, std::string name, double x, double y, double z, double size, double mass);
  StartState(int type, std::string name, double x, double y, double z, double thetaX, 
        double thetaY, double thetaZ, double size, double mass);
  StartState(int type, std::string name, double x, double y, double z, double thetaX, 
        double thetaY, double thetaZ, double size, double mass, double cFriccion, double cRestitution);
  virtual ~StartState();

  std::string getName() { return name_; };
  int getType() { return type_; };
  double getX() { return x_; };
  double getY() { return y_; };
  double getZ() { return z_; };
  double getTheta() { return thetaY_; };
  double getThetaX() { return thetaX_; };
  double getThetaZ() { return thetaZ_; };
  double getSize() { return size_; };
  double getMass() { return mass_; };
  double getFriction() { return cFriction_; };
  double getRestitution() { return cRestitution_; };

  void setName(std::string name) { name_ = name; };
  void setX(double x) { x_ = x; };
  void setY(double y) { y_ = y; };
  void setZ(double z) { z_ = z; };
  void setTheta(double thetaY) { thetaY_ = thetaY; };
  void setThetaX(double thetaX) { thetaX_ = thetaX; };
  void setThetaZ(double thetaZ) { thetaZ_ = thetaZ; };

private:
  int type_;
  std::string name_;
  double x_;
  double y_;
  double z_;
  double thetaX_;
  double thetaY_;
  double thetaZ_;
  double size_;
  double mass_;
  double cFriction_;
  double cRestitution_;
};

}
#endif /* STARTSTATE_H_ */
