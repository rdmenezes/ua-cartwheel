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
  theta_ = theta;
}

StartState::~StartState()
{
  // TODO Auto-generated destructor stub
}

}
