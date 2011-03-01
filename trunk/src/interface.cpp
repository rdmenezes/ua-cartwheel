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
  actions.push_back(new WalkAction(5.0, 0.5));
  actions.push_back(new WalkAction(5.0, -0.5));
  actions.push_back(new WalkAction(6.0, 0.0));

  vector<PosState*>* trajectory = interface.simulate(start_state, actions);
  for (vector<PosState*>::iterator it = trajectory->begin(); it != trajectory->end(); ++it)
  {
    // TODO: Print out trajectory
//    for (int i = 0; i < (*it)->getNumVectors(); ++i)
//    {
//      cout << (*it)->getPosition(i)
//    }
  }
}
