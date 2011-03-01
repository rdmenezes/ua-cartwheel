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
SimulationInterface::SimulationInterface(bool visualize)
{
  this->visualize = visualize;
  sPath = new char[200];
  strcpy(sPath, "");
  simulator = new CartWheel3D(sPath);

  if (visualize)
  {
    int argc = 0;
    char** argv = NULL;
    visualization = new Visualization(render, argc, argv, 640, 480);

    Point3d camerPos(0.0, 5.0, 5.0);
    Point3d cameraTarget(0.0, 1.0, 0.0);
    Point3d cameraUp(0.0, 1.0, 0.0);

    visualization->initGL(camerPos, cameraTarget);
    visualization->setCameraLocation(camerPos);
    visualization->setCameraTarget(cameraTarget);
    visualization->setCameraUp(cameraUp);

    visualization->setRenderGround(true);
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
  simulator->addObject("ground", "data/objects/flatGround.rbs", -1);

  //simulator->addObject("dodgeBox", "data/objects/box.rbs", 1);

  string humanModel = "data/characters/bipV3.rbs";
  string humanController = "data/controllers/bipV3/Walking.sbc";

  Point3d p1(start_state[0], 1.0, start_state[1]);
  Point3d p2(start_state[3], 1.0, start_state[4]);

  simulator->addHuman(humanModel, humanController, p1, start_state[2]);
  simulator->addHuman("data/characters/bip2V3.rbs", humanController, p2, start_state[5]);
  simulator->setHumanSpeed(1, 0);
}

std::vector<PosState*>* SimulationInterface::simulate(std::vector<double> start_state, std::vector<TomsAction*> actions)
{
  double steps_per_second = 2000.0;
  double step_size = 1 / steps_per_second;

  init_simulation(start_state);
  cout << "HERE" << endl;

  // This vector will store the trajectory
  std::vector<PosState*> *trajectory = new std::vector<PosState*>();

  double total_time = 0.0;
  for (std::vector<TomsAction*>::iterator it = actions.begin(); it != actions.end(); ++it)
  {
    total_time += (*it)->getTime();
  }
  total_time;

  // Queue up the first action
  int action_index = 0;
  TomsAction* curr_action = actions[action_index];
  curr_action->executeSetup(simulator);
  double prev_start = 0.0;
  // Run each action for it's duration
  for (double curr_time = 0.0; curr_time < total_time; curr_time += step_size * 200) // Not sure about this constant
  {
    if (curr_time > curr_action->getTime() + prev_start) // rename to duration
    {
      if (action_index == actions.size() - 1)
      {
        break;
      }
      cout << "SWITCHING ACTION" << endl;
      action_index++;
      curr_action = actions[action_index];
      curr_action->executeSetup(simulator);
      prev_start = curr_time;
    }

    PosState* pos = new PosState(simulator);
    trajectory->push_back(pos);

    curr_action->executeStep(simulator, step_size);
    if (visualize)
    {
      visualization->render(simulator);
    }
  }

  return trajectory;
}
