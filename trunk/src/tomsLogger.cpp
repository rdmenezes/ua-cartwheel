#include <Control/WrapperAction.h>
#include <Control/SimulationInterface.h>
#include <vector>
#include <iostream>
#include <Control/ActRecognizerInterface.h>
#include <boost/assign/std/vector.hpp>


using std::vector;
using std::cout;
using std::endl;


using namespace std;
using namespace CartWheel;
using namespace CartWheel::Core;
using namespace boost::assign;


#define ACTREC

int main(int argc, char** argv)
{
  SimulationInterface interface(false);//true);

  StartStatePtr s1(new StartState("Human1", 2, 2, -3.14 / 1.5));
  vector<StartStatePtr> start_state;
  start_state += s1;

for(double curSpeed = 0.0; curSpeed <= 3.0; curSpeed += 0.25){
 
 
  vector<double> params1;
  
  params1 += 15.0;  //time 
  params1 += curSpeed;  //speed

  vector<ExtendedActionPtr> actions1;
  ExtendedActionPtr a1(new WrapperAction("walk", params1));
  actions1 += a1;

  for(int i =0; i < actions1.size(); i++){
  	actions1[i]->setActor("Human1"); 
  }  

   vector<vector<ExtendedActionPtr> > actions;
   actions += actions1;

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
      cout <<sqrt((pos_state->getPosition(j).x - trajectory[0]->getPosition(0).x) * (pos_state->getPosition(j).x - trajectory[0]->getPosition(0).x) + (pos_state->getPosition(j).z - trajectory[0]->getPosition(0).z) * (pos_state->getPosition(j).z - trajectory[0]->getPosition(0).z))  <<",";
    //}
  }
  cout<<endl;
  //delete actions[0];
}

}
