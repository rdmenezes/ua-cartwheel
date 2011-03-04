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
#include <Control/ExtendedAction.h>
#include <Control/RelationalState.h>
#include <Control/PosState.h>
#include <Control/CapsuleState.h>
#include <Control/RelationalState.h>


class SimulationInterface
{
public:
  SimulationInterface(bool visualize);
  virtual ~SimulationInterface();

  void init_simulation(std::vector<double> start_state);
  void simulate(std::vector<double> start_state, std::vector<ExtendedAction*> actions);

  const std::vector<PosState*>& getPositions() const;
  const std::vector<CapsuleState*>& getCapsules()  const;
  const std::vector<RelationalState*>& getRelations() const;
  std::vector<string>* getLastHumanNames(){return storedNames_;};

private:
  CartWheel3D* simulator_;
  Visualization* visualization_;
  bool visualize_;
  char* sPath_;

  std::vector<std::string>* storedNames_;
  std::vector<RelationalState*> relations_;
  std::vector<PosState*> positions_;
  std::vector<CapsuleState*> capsules_;
};

#endif /* SIMULATIONINTERFACE_H_ */
