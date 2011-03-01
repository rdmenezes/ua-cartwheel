#include <Control/WalkAction.h>;
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
  start_state.push_back(0);
  start_state.push_back(-2);
  start_state.push_back(3.14 / 2.0);
  start_state.push_back(0);
  start_state.push_back(0);
  start_state.push_back(0);

  vector<ExtendedAction*> actions;
  actions.push_back(new WalkAction(10.0, 0.5));
  actions.push_back(new WalkAction(10.0, -0.5));
  actions.push_back(new WalkAction(10.0, 0.0));

  interface.simulate(start_state, actions);
  vector<PosState*> trajectory = interface.getPositions();
  cout << "GOT " << trajectory.size() << " STATES" << endl;
  for (vector<PosState*>::iterator it = trajectory.begin(); it != trajectory.end(); ++it)
  {
    cout << "POSITIONS:" << endl;
    // TODO: Print out trajectory
    for (int i = 0; i < (*it)->getNumVectors(); ++i)
    {
      cout << (*it)->getName(i) << ": " << (*it)->getPosition(i).x << " " << (*it)->getPosition(i).z << endl;
    }
  }
}
