#include <Control/WrapperAction.h>
#include <Control/SimulationInterface.h>
#include <vector>
#include <iostream>
#include <Control/ActRecognizerInterface.h>

using std::vector;
using std::cout;
using std::endl;

using namespace std;

#define ACTREC

int main(int argc, char** argv)
{
 

  SimulationInterface interface(true);//true);

  vector<double> start_state;
  start_state.push_back(2);
  start_state.push_back(2);
  start_state.push_back(-3.14 / 1.5);
  start_state.push_back(-1);
  start_state.push_back(-1);
  start_state.push_back(0);

  vector<double> params1;
  vector<double> params2;
  vector<double> params3;

  params1.push_back(10.0);
  params1.push_back(0.5);
   params2.push_back(10.0);
  params2.push_back(-0.5);
 params3.push_back(10.0);
  params3.push_back(0.0);


  vector<ExtendedAction*> actions;
  actions.push_back(new WrapperAction(std::string("walk"), params1));//new WalkAction(10.0, 0.5));
  actions.push_back(new WrapperAction(std::string("walk"), params2));//new WalkAction(10.0, -0.5));
  actions.push_back(new WrapperAction(std::string("walk"), params3));//new WalkAction(10.0, 0.0));

   for(int i =0; i < actions.size(); i++){
  	actions[i]->setActor("Human1"); 
  }  

  interface.simulate(start_state, actions);
  vector<PosState*> trajectory = interface.getPositions();
  vector<CapsuleState*> capsule_states = interface.getCapsules();
  vector<RelationalState*> rel_states = interface.getRelations();
  cout << "GOT " << trajectory.size() << " STATES" << endl;
  
  #ifdef ACTREC
      vector<string>* namers= interface.getLastHumanNames();

      ActRecognizerInterface ari(string("meet"), *namers);
  #endif

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
    #ifdef ACTREC
       ari.progress(*(rel_states[i]));
       if(ari.getCurTerminal())
	cout<<ari.getFullVerbName()<<" achieved at step "<<i<<endl;
     #endif


    cout << "CAPSULES:" << endl;
    for (int j = 0; j < cap_state->getNumEntities(); ++j)
    {
      cout << cap_state->getName(j) << ": " << cap_state->getCapsules(j).size() << endl;
    }
  }

  cout << "running a second time..." << endl;
  interface.simulate(start_state, actions);

}
