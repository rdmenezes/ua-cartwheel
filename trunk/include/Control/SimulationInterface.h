/*
 * SimulationInterface.h
 *
 *  Created on: Mar 1, 2011
 *      Author: dhewlett
 */

#ifndef SIMULATIONINTERFACE_H_
#define SIMULATIONINTERFACE_H_

#include <Core/CartWheel3D.h>
#include <Core/Visualization.h>
#include <Control/StaticWalkNearPolicy.h>
#include <Control/TomsAction.h>
#include <Control/RelationalState.h>
#include <Control/PosState.h>

#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <algorithm>

class SimulationInterface
{
public:
  SimulationInterface(bool visualize);
  virtual ~SimulationInterface();

//  void render(void);
  void init_simulation(std::vector<double> start_state);
  std::vector<PosState*>* simulate(std::vector<double> start_state, std::vector<TomsAction*> actions);
private:
  CartWheel3D* simulator;
  Visualization* visualization;
  bool visualize;
  char* sPath;
};

#endif /* SIMULATIONINTERFACE_H_ */
