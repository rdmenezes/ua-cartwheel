/*
 * SimulationInterface.cpp
 *
 *  Created on: Mar 1, 2011
 *      Author: dhewlett
 */

#include <Control/SimulationInterface.h>

void render(void)
{
  // Does nothing
}

// TODO This will probably need a parameter for sPath
SimulationInterface::SimulationInterface(bool visualize):relations_(),positions_(),capsules_()
{
  visualize_ = visualize;
  sPath_ = new char[200];
  strcpy(sPath_, "");
  simulator_ = new CartWheel3D(sPath_);

  if (visualize_)
  {
    Point3d camerPos(0.0, 5.0, 5.0);
    Point3d cameraTarget(0.0, 1.0, 0.0);
    Point3d cameraUp(0.0, 1.0, 0.0);

    int argc = 0;
    char** argv = NULL;
    visualization_ = new Visualization(render, argc, argv, 640, 480);
    visualization_->initGL(camerPos, cameraTarget);
    visualization_->setCameraLocation(camerPos);
    visualization_->setCameraTarget(cameraTarget);
    visualization_->setCameraUp(cameraUp);
    visualization_->setRenderGround(true);
    visualization_->setCartWheelHandle(simulator_);
  }
  else
  {
    // Do nothing.
  }
}

SimulationInterface::~SimulationInterface()
{
}

void SimulationInterface::init_simulation(std::vector<double> start_state)
{
  positions_.clear();
  capsules_.clear();
  relations_.clear();

  simulator_->addObject("ground", "data/objects/flatGround.rbs", -1);

  //simulator->addObject("dodgeBox", "data/objects/box.rbs", 1);

  string humanModel = "data/characters/bipV3.rbs";
  string humanController = "data/controllers/bipV3/Walking.sbc";
//  string humanController2 = "data/controllers/bipV3/Walking2.sbc";

  Point3d p1(start_state[0], 1.0, start_state[1]);
  Point3d p2(start_state[3], 1.0, start_state[4]);

  // Add human 1
  string humanName1 = "Human1";
  simulator_->addHuman(humanName1, humanModel, humanController, p1, start_state[2]);

  // Add human 2
  string humanName2 = "Human2";
  simulator_->addHuman(humanName2, "data/characters/bipV3.rbs", humanController, p2, start_state[5]);
  simulator_->setHumanSpeed(humanName2, 0);
}

void SimulationInterface::simulate(std::vector<double> start_state, std::vector<ExtendedAction*> actions)
{
  int steps_per_second = 2000;
  double step_size = 1.0 / steps_per_second;

  double other_steps_per_second = 200;

  int samples_per_second = 2;
  int sampling_rate = other_steps_per_second / (samples_per_second * 20);

  init_simulation(start_state);

  double total_time = 0.0;
  for (std::vector<ExtendedAction*>::iterator it = actions.begin(); it != actions.end(); ++it)
  {
    total_time += (*it)->getTime();
  }
  total_time;

  // Queue up the first action
  int action_index = 0;
  ExtendedAction* curr_action = actions[action_index];
  curr_action->executeSetup(simulator_);
  double prev_start = 0.0;
  // Run each action for it's duration
  int i = 0;
  for (double curr_time = 0.0; curr_time < total_time; curr_time += step_size * other_steps_per_second) // Not sure about this constant
  {
    if (curr_time > curr_action->getTime() + prev_start) // rename to duration
    {
      if (action_index == actions.size() - 1)
      {
        break;
      }
//      cout << "SWITCHING ACTION" << endl;
      action_index++;
      curr_action = actions[action_index];
      curr_action->executeSetup(simulator_);
      prev_start = curr_time;
    }

    if (i % sampling_rate == 0)
    {
      PosState* pos_state = new PosState(simulator_);
      positions_.push_back(pos_state);
      CapsuleState* capsule_state = new CapsuleState(simulator_);
      capsules_.push_back(capsule_state);
      RelationalState* rel_state = new RelationalState();
      if(i != 0)
          rel_state->reset(*positions_[(i / sampling_rate)-1],simulator_);
      relations_.push_back(rel_state);
    }

    curr_action->executeStep(simulator_, step_size);
    if (visualize_)
    {
      visualization_->render(simulator_);
      visualization_->glutStep();
    }

    ++i;
  }
  simulator_->reset();
}

const std::vector<RelationalState*>& SimulationInterface::getRelations() const
{
  return relations_;
}

const std::vector<PosState*>& SimulationInterface::getPositions() const
{
  return positions_;
}

const std::vector<CapsuleState*>& SimulationInterface::getCapsules() const
{
  return capsules_;
}
