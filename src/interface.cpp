#include <Control/WalkAction.h>
#include <Control/SimulationInterface.h>
#include <vector>
#include <iostream>

using std::vector;
using std::cout;
using std::endl;

int main(int argc, char** argv)
{
  SimulationInterface interface(false);//true);

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
  vector<RelationalState*> rel_states = interface.getRelations();
  cout << "GOT " << trajectory.size() << " STATES" << endl;
  for (int i = 0; i < trajectory.size(); ++i)
  {
    PosState* pos_state = trajectory[i];
    CapsuleState* cap_state = capsule_states[i];

    cout << "STATE " << i << "\n=============================\n";
    cout << "POSITIONS:" << endl;
    for (int j = 0; j < pos_state->getNumVectors(); ++j)
    {
      cout << pos_state->getName(j) << ": " << pos_state->getPosition(j).x << " " << pos_state->getPosition(j).z
          << endl;

    }
    cout << "RELATIONS " << i << "\n=============================\n";
    cout << rel_states[i]->toString()  <<endl;



    cout << "CAPSULES:" << endl;
    for (int j = 0; j < cap_state->getNumEntities(); ++j)
    {
      cout << cap_state->getName(j) << ": " << cap_state->getCapsules(j).size() << endl;
    }
  }

  cout << "running a second time..." << endl;
  interface.simulate(start_state, actions);

}
