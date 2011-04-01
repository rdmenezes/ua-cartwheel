#include <Control/WrapperAction.h>
#include <Control/SimulationInterface.h>
#include <vector>
#include <iostream>
#include <Control/ActRecognizerInterface.h>

using std::vector;
using std::cout;
using std::endl;

using namespace std;
using namespace CartWheel;
using namespace CartWheel::Core;

#define ACTREC

int main(int argc, char** argv)
{
  SimulationInterface interface(false);//true);

  vector<double> start_state;
  start_state.push_back(2.0);
  start_state.push_back(2.0);
  start_state.push_back(-3.14);

for(double curSpeed = 0.0; curSpeed <= 3.0; curSpeed += 0.25){
  
  vector<double> params1;
  
  params1.push_back(5.0);  //time 4.1
  params1.push_back(curSpeed);  //speed

  vector<ExtendedAction*> actions;
  actions.push_back(new WrapperAction(std::string("walk"), params1));

  for(int i =0; i < actions.size(); i++){
  	actions[i]->setActor("Human1"); 
  }  

  interface.simulate(start_state, actions);
  vector<PosState*> trajectory = interface.getPositions();
  //cout << "GOT " << trajectory.size() << " STATES" << endl;
  
  cout<<curSpeed<<",";

  for (int i = 0; i < trajectory.size(); ++i)
  {
    PosState* pos_state = trajectory[i];

    //cout << "STATE " << i << "\n=============================\n";
    //cout << "POSITIONS:" << endl;
        int j = 0;
    //for (int j = 0; j < pos_state->getNumVectors(); ++j)
    //{
      cout <<sqrt((pos_state->getPosition(j).x - 2.0) * (pos_state->getPosition(j).x - 2.0) + (pos_state->getPosition(j).z - 2.0) * (pos_state->getPosition(j).z - 2.0))  <<",";
    //}
  }
  cout<<endl;
  delete actions[0];
}

}
