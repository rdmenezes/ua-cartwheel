/*
 * SimulationInterface.cpp
 *
 *  Created on: Mar 1, 2011
 *      Author: dhewlett
 */

#include <Control/SimulationInterface.h>
#include <Control/WrapperAction.h>

//#include <boost/assign/std/vector.hpp>
//
//using namespace boost::assign;

using std::vector;

namespace CartWheel
{
void render(void)
{
  // Does nothing
}

// TODO This will probably need a parameter for sPath
SimulationInterface::SimulationInterface(bool visualize) :
  relations_(), positions_(), capsules_(), visualizeCapsuleCallback_(NULL)
{
  visualize_ = visualize;
  //sPath_ = new char[200];
  //strcpy(sPath_, "");
  simulator_ = new CartWheel3D(s_path_); // Empty string
  storedNames_ = vector<string> ();
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
  delete simulator_;
  if (visualize_)
    delete visualization_;
}

void SimulationInterface::fullClear()
{
  for (int i = 0; i < positions_.size(); i++)
  {
    delete positions_[i];
    delete relations_[i];
    delete capsules_[i];
  }
  positions_.clear();
  capsules_.clear();
  relations_.clear();
  storedNames_.clear();

}

void SimulationInterface::init_simulation(vector<StartStatePtr> const &start_state)
{
  fullClear();

  simulator_->addObject("ground", "data/objects/flatGround.rbs", -1);

  //simulator->addObject("dodgeBox", "data/objects/box.rbs", 1);

  string humanModel = "data/characters/bipV3.rbs";
  string humanController = "data/controllers/bipV3/HMV/compositeController.con";
  string humanAction = "data/controllers/bipV3/HMV/actions";

  int i = 0;
  for (vector<StartStatePtr>::const_iterator ss = start_state.begin(); ss != start_state.end(); ++ss)
  {
    Point3d p1((*ss)->getX(), 1.0, (*ss)->getZ());
    string humanName = (*ss)->getName();
    storedNames_.push_back(humanName);
    simulator_->addHuman(humanName, humanModel, humanController, humanAction, p1, (*ss)->getTheta());
    simulator_->setHumanSpeed(humanName, 0.0);

    ++i;
  }
}

// Simulation terminates when all actors have run out of actions
// "Dead space" in simulation is padded with standing still
void SimulationInterface::simulate(vector<StartStatePtr> const &start_state,
                                   vector<vector<ExtendedActionPtr> > const &actions)
{
  int steps_per_second = 2000;
  double step_size = 1.0 / steps_per_second;

  int samples_per_second = 2;  //change back to 2!!!
  int steps_per_sample = steps_per_second / samples_per_second;

  int visual_per_second = 30;
  int steps_per_visual = steps_per_second / visual_per_second;

  init_simulation(start_state);

  // Find the total duration that we will run for, as well as the total duration of each person's actions
  double total_time = 0.0;
  double durations[actions.size()];
  for (int i = 0; i < actions.size(); i++)
  {
    durations[i] = 0.0;
    vector<ExtendedActionPtr> my_actions = actions[i];
    for (vector<ExtendedActionPtr>::const_iterator it = my_actions.begin(); it != my_actions.end(); ++it)
    {
      durations[i] += (*it)->getTime(); // Really rename to duration
    }
    if (durations[i] > total_time)
    {
      total_time = durations[i];
    }
  }

  int action_index[actions.size()];
  double prev_starts[actions.size()];
  bool complete[actions.size()];

  for (int i = 0; i < actions.size(); i++)
  {
    prev_starts[i] = 0.0;

    if (actions[i].empty()) // Stand still the whole time
    {
      vector<double> params;
      params.push_back(total_time);
      ExtendedActionPtr null_action(new WrapperAction("standStill", start_state[i]->getName(), params));
      null_action->executeSetup(simulator_);
      complete[i] = true;
    }
    else
    {
      action_index[i] = 0;
      ExtendedActionPtr curr_action = actions[i][0];
      curr_action->executeSetup(simulator_); // Set it up
      complete[i] = false;
    }
  }

  cout << "Total Time: " << total_time << " Step Size: " << step_size << endl;

  // Outer loop: Time
  int i = 0; // step counter
  double curr_time = 0.0;
  for (curr_time = 0.0; curr_time < total_time; curr_time += step_size) 
  {
     // cout << "Iteration: " << i << " " << curr_time << endl;
    bool keep_going = false;
    // Inner loop: Humans
    for (int h = 0; h < actions.size(); h++)
    {
      if (complete[h])
      {
        continue;
      }

      // Check whether the action is over
      if (curr_time > actions[h][action_index[h]]->getTime() + prev_starts[h]) // rename action.time to duration
      {
        cout << " Action finished " << endl;
        if (action_index[h] == actions[h].size() - 1)
        {
          cout << start_state[h]->getName() << " " << durations[h] << " " << total_time << endl;

          if (durations[h] < total_time) // If this guy is finished but others are still going, pad with stand still
          {
            vector<double> params;
            params.push_back(total_time - durations[h]);
            ExtendedActionPtr null_action(new WrapperAction("standStill", start_state[h]->getName(), params));
            null_action->executeSetup(simulator_);
          }

          complete[h] = true;
          continue;
        }

        // If so, move on to the next action
        action_index[h]++;
        // Setup the next action
        actions[h][action_index[h]]->executeSetup(simulator_);
        // Store the start time of the current action
        prev_starts[h] = curr_time;
      }

      keep_going = true;
    }
    // END HUMAN LOOP

    if (!keep_going)
    {
      break; // Termination condition
    }

    // Step the simulator
    simulator_->runStep(step_size);
    // ... and update the step counter
    ++i;

    if (i % steps_per_sample == 0)
    {
      PosState* pos_state = new PosState(simulator_);
      positions_.push_back(pos_state);
      CapsuleState* capsule_state = new CapsuleState(simulator_);
      capsules_.push_back(capsule_state);
      RelationalState* rel_state = new RelationalState();
//      if (i != 0)
//      {
//        rel_state->reset(*positions_[(i / sampling_rate) - 1], simulator_);
//      }
      relations_.push_back(rel_state);
    }

    // Lastly, render (if visualization is on)
    if (visualize_ && i % steps_per_visual == 0)
    {
        //cout << "Render " << i << " current time " << curr_time << endl;
      if (visualizeCapsuleCallback_)
      {
        CartWheel::CapsuleState* capsule_state = new CapsuleState(simulator_);
        visualizeCapsuleCallback_(*capsule_state);
        delete capsule_state; // Ummm...
      }
      visualization_->render(simulator_);
      visualization_->glutStep();
    }

  }

  cout << "Simulation Time: " << curr_time << " Steps: " << i << endl;

  // Reset the simulator for the next run
  simulator_->reset();
}

const vector<RelationalState*>& SimulationInterface::getRelations() const
{
  return relations_;
}

const vector<PosState*>& SimulationInterface::getPositions() const
{
  return positions_;
}

const vector<CapsuleState*>& SimulationInterface::getCapsules() const
{
  return capsules_;
}

} // namespace CartWheel
