/*
 * StartState.cpp
 *
 *  Created on: Apr 5, 2011
 *      Author: dhewlett
 */

#include <Control/StartState.h>

namespace CartWheel {

StartState::StartState(std::string name, double x, double z, double theta)
{
  name_ = name;
  x_ = x;
  z_ = z;
  thetaY_ = theta;
  
  type_ = 0;
}

StartState::StartState(int type, std::string name, double x, double y, double z, 
        double size, double mass)
{
  type_ = type;  
  name_ = name;
  x_ = x;
  y_ = y;
  z_ = z;
  size_ = size;
  mass_ = mass;
  
  thetaX_ = 0;
  thetaY_ = 0;
  thetaZ_ = 0;  
  cFriction_ = -1;
  cRestitution_ = -1;
}

StartState::StartState(int type, std::string name, double x, double y, double z, 
        double thetaX, double thetaY, double thetaZ, double size, double mass)
{
  type_ = type;  
  name_ = name;
  x_ = x;
  y_ = y;
  z_ = z;
  thetaX_ = thetaX;
  thetaY_ = thetaY;
  thetaZ_ = thetaZ;
  size_ = size;
  mass_ = mass;
  cFriction_ = -1;
  cRestitution_ = -1;
}

StartState::StartState(int type, std::string name, double x, double y, double z, 
        double thetaX, double thetaY, double thetaZ, double size, double mass,
        double cFriction, double cRestitution)
{
  type_ = type;  
  name_ = name;
  x_ = x;
  y_ = y;
  z_ = z;
  thetaX_ = thetaX;
  thetaY_ = thetaY;
  thetaZ_ = thetaZ;
  size_ = size;
  mass_ = mass;
  cFriction_ = cFriction;
  cRestitution_ = cRestitution;
}

StartState::~StartState()
{
  // TODO Auto-generated destructor stub
}

}
