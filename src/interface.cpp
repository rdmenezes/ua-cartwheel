#include <Control/WrapperAction.h>
#include <Control/SimulationInterface.h>
#include <vector>
#include <iostream>
#include <Control/ActRecognizerInterface.h>
#include <boost/assign/std/vector.hpp>

using std::vector;
using std::cout;
using std::endl;

using namespace CartWheel;
using namespace CartWheel::Core;
using namespace boost::assign;

//#define ACTREC

int main(int argc, char** argv) {
    SimulationInterface interface(true);

    string actor1 = "Human1";
    string actor2 = "Human2";
    string actor3 = "Ball1";

    // NEW
    //  StartStatePtr s1(new StartState(actor1, 2, 2, -3.14 / 1.5));
    StartStatePtr s1(new StartState(actor1, 0, -1, 0));
    StartStatePtr s2(new StartState(actor2, -1, -3, 0.0));
    //ObjType (1=Ball), Name, posX, posY, posZ, size, mass
    StartStatePtr s3(new StartState(1, actor3, -1, 1, -1, 0.1, 0.1));

    vector<StartStatePtr> start_state;
    start_state += s1, s3; //, s2;

    // FIRST HUMAN
//    vector<double> params1;
//    params1 += 50.0, 0.0;

//    vector<double> params2;
//    params2 += 5.0, -0.3; //no second param for standStill
    
    
    
    vector<double> params1; //Walk Params
    params1 += 3.0, 0.2;
    
    vector<double> params2; //WalkInArc Params
    //Duration, WalkSpeed, AngSpeed(rad/sec)
    params2 += 5.0, 0.4, 0.6;
    
//    double dTime = 15/4;
    vector<double> params3; //WalkInPath Params
    //<x, z, t>[1], ... <x, z, t>[N]
    params3 += 0, 0, 1,   1, 0, 3,   -1, 1, 3,   3, 1, 5;
    
    vector<double> params4; //WaveHand Params
    //Duration, none
    params4 += 3.0, 0;
    

    vector<ExtendedActionPtr> actions1;
    ExtendedActionPtr a1(new WrapperAction("walk", actor1, params1));
    ExtendedActionPtr a2(new WrapperAction("WalkInArc", actor1, params2));
    ExtendedActionPtr a3(new WrapperAction("WalkInPath", actor1, params3));
    ExtendedActionPtr a4(new WrapperAction("WaveHand", actor1, params4));
    actions1 += a4, a3;
    
    
    
////    vector<double> params1; //Bounce Params
////    //Duration, Position(x, y, z), Velocity(x, y, z)
////    params1 += 2.0, 1, 1, 1, -2, 2, -2;
    
////    vector<ExtendedActionPtr> actionsObj;
////    ExtendedActionPtr a6(new WrapperAction("MoveBall", actor3, params1));
////    actionsObj += a6;
    

    //ExtendedActionPtr a2(new WrapperAction("standStill", actor1, params2));
//    // SECOND HUMAN
//    vector<double> params4;
//    params4 += 5.4, 0.53;
//    ExtendedActionPtr a4(new WrapperAction("walk", actor2, params4));
//
//    vector<double> params5;
//    params5 += 10.4; //no second param for standStill
//    ExtendedActionPtr a5(new WrapperAction("standStill", actor2, params5));
//
//    vector<ExtendedActionPtr> actions2;
//    actions2 += a4; // Partial duration, comment out for empty action vector
//    //  actions2 += a5; // Comment in for full duration

    vector<vector<ExtendedActionPtr> > actions;
    actions += actions1;//, actionsObj; //, actions2;


    ////////////////////////////////////////
    // BOXES
    Vector3d box_scale(0.25, 0.15, 0.15);
    Vector3d box_pos(-2.0, 0.10, 1.0);
    BoxStatePtr box1(new BoxState("box1", box_pos, 3 * 3.14 / 4.0, box_scale, 4.0));

    vector<BoxStatePtr> boxes;
    boxes += box1;

    ////////////////////////////////////////
    // SIMULATE
    //  interface.simulate(start_state, actions);
    interface.simulate(start_state, boxes, actions);

    vector<PosState*> trajectory = interface.getPositions();
    vector<CapsuleState*> capsule_states = interface.getCapsules();
    vector<RelationalState*> rel_states = interface.getRelations();
    cout << "GOT " << trajectory.size() << " STATES" << endl;

#ifdef ACTREC
    const vector<string>& namers = interface.getLastHumanNames();

    ActRecognizerInterface ari(string("meet"), namers);
#endif

    for (int i = 0; i < trajectory.size(); ++i) {
        PosState* pos_state = trajectory[i];
        CapsuleState* cap_state = capsule_states[i];

        cout << "STATE " << i << "\n=============================\n";
        cout << "POSITIONS:" << endl;
        for (int j = 0; j < pos_state->getNumVectors(); ++j) {
            cout << pos_state->getName(j) << ": " << pos_state->getPosition(j).x << " " << pos_state->getPosition(j).z
                    << endl;

        }
        cout << "BOXES:" << endl;
        vector<BoxStatePtr> boxes = pos_state->getBoxStates();
        for (vector<BoxStatePtr>::const_iterator bx = boxes.begin(); bx != boxes.end(); ++bx) {
            cout << (*bx)->getName() << " => (" << (*bx)->getPosition().x << "," << (*bx)->getPosition().y << ") " << (*bx)->getQuaternion().getAngle() << endl;
        }

        cout << "RELATIONS " << i << "\n=============================\n";
        cout << rel_states[i]->toString() << endl;

#ifdef ACTREC
        ari.progress(*(rel_states[i]));
        if (ari.getCurTerminal())
            cout << ari.getFullVerbName() << " achieved at step " << i << endl;
#endif

        cout << "CAPSULES:" << endl;
        for (int j = 0; j < cap_state->getNumEntities(); ++j) {
            cout << cap_state->getName(j) << ": " << cap_state->getCapsules(j).size() << endl;
        }
    }

    //  cout << "running a second time..." << endl;
    //  interface.simulate(start_state, actions);

}
