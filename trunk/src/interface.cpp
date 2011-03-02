#include <Control/WalkAction.h>
#include <Control/SimulationInterface.h>
#include <vector>
#include <iostream>

using std::vector;
using std::cout;
using std::endl;

int main(int argc, char** argv)
{
  SimulationInterface interface(true);

  vector<double> start_state;
  start_state.push_back(2);
  start_state.push_back(2);
  start_state.push_back(-3.14 / 1.5);
  start_state.push_back(-1);
  start_state.push_back(-1);
  start_state.push_back(0);

  vector<ExtendedAction*> actions;
  actions.push_back(new WalkAction(10.0, 0.5));
  actions.push_back(new WalkAction(10.0, -0.5));
  actions.push_back(new WalkAction(10.0, 0.0));

  interface.simulate(start_state, actions);
  vector<PosState*> trajectory = interface.getPositions();
  vector<CapsuleState*> capsule_states = interface.getCapsules();
  cout << "GOT " << trajectory.size() << " STATES" << endl;
  for (int i = 0; i < trajectory.size(); ++i)
  {
    PosState* pos_state = trajectory[i];
    CapsuleState* cap_state = capsule_states[i];

    cout << "STATE " << i << "\n=============================\n";
    cout << "POSITIONS:" << endl;
    for (int i = 0; i < pos_state->getNumVectors(); ++i)
    {
      cout << pos_state->getName(i) << ": " << pos_state->getPosition(i).x << " " << pos_state->getPosition(i).z
          << endl;

    }
    cout << "CAPSULES:" << endl;
    for (int i = 0; i < cap_state->getNumEntities(); ++i)
    {
      cout << cap_state->getName(i) << ": " << cap_state->getCapsules(i).size() << endl;
    }
  }

  cout << "running a second time..." << endl;
  interface.simulate(start_state, actions);

}
