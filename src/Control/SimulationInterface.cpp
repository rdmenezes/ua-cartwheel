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

namespace CartWheel {

    void render(void) {
        // Does nothing
    }

    SimulationInterface::SimulationInterface(bool visualize) :
    relations_(), positions_(), capsules_(), visualizeCapsuleCallback_(NULL) {
        visualize_ = visualize;
        simulator_ = new CartWheel3D(s_path_); // Empty string
        storedNames_ = vector<string > ();
        if (visualize_) {
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
        } else {
            // Do nothing.
        }
    }

    SimulationInterface::~SimulationInterface() {
        delete simulator_;
        if (visualize_)
            delete visualization_;
    }

    void SimulationInterface::fullClear() {
        // This really needs to be modernized sometime!
        for (int i = 0; i < positions_.size(); i++) {
            delete positions_[i];
            delete relations_[i];
            delete capsules_[i];
        }
        positions_.clear();
        capsules_.clear();
        relations_.clear();
        storedNames_.clear();
    }

    void SimulationInterface::init_simulation(vector<StartStatePtr> const& start_state,
            vector<BoxStatePtr> const& boxes) {
        fullClear();

        simulator_->addObject("ground", "data/objects/flatGround.rbs", -1);

        for (vector<BoxStatePtr>::const_iterator bx = boxes.begin(); bx != boxes.end(); ++bx) {
            (*bx)->addToWorld(simulator_);
        }

        string humanModel = "data/characters/bipV3.rbs";
        string humanController = "data/controllers/bipV3/HMV/compositeController.con";
        string humanAction = "data/controllers/bipV3/HMV/actions";

        for (vector<StartStatePtr>::const_iterator ss = start_state.begin(); ss != start_state.end(); ++ss) {
            if((*ss)->getType() == 0) { //Human
                Point3d p1((*ss)->getX(), 1.0, (*ss)->getZ());
                string humanName = (*ss)->getName();
                storedNames_.push_back(humanName);
                simulator_->addHuman(humanName, humanModel, humanController, humanAction, p1, (*ss)->getTheta());
                simulator_->setHumanSpeed(humanName, 0.0);
            } else if((*ss)->getType() == 1) { //Ball Object
                Point3d p1((*ss)->getX(), (*ss)->getY(), (*ss)->getZ());
                Vector3d size((*ss)->getSize(),(*ss)->getSize(),(*ss)->getSize());
                string objName = (*ss)->getName();
                storedNames_.push_back(objName);
                simulator_->addBehaviorObject(objName, "", size, (*ss)->getMass(), (*ss)->getFriction(), (*ss)->getRestitution());
                simulator_->getObjectByName(objName)->setCMPosition(p1);
            }
        }
    }

    void SimulationInterface::simulate(vector<StartStatePtr> const &start_state, vector<BoxStatePtr> const& boxes,
            vector<vector<ExtendedActionPtr> > const &actions) {
        int steps_per_second = 2000;
        double step_size = 1.0 / steps_per_second;
        SimGlobals::steps_per_second = steps_per_second;

        int samples_per_second = 2; //change back to 2!!!
        int steps_per_sample = steps_per_second / samples_per_second;

        int visual_per_second = 30;
        int steps_per_visual = steps_per_second / visual_per_second;

        init_simulation(start_state, boxes);

        // Initialize all Actions with their parameters
        double total_time = 0.0;
        for (int i = 0; i < actions.size(); i++) {
            vector<ExtendedActionPtr> my_actions = actions[i];
            for (vector<ExtendedActionPtr>::const_iterator it = my_actions.begin(); it != my_actions.end(); ++it) {
                (*it)->executeSetup(simulator_);
            }
        }
        total_time = simulator_->getTotalSerialTime();
        cout << "Total Time: " << total_time << ", Step Size: " << step_size << endl;
        
        
//        for(int i=0; i<simulator_->getWorld()->getRBCount(); i++) {
//            std::string sObjs = simulator_->getWorld()->getRB(i)->getName()==NULL ? "" : simulator_->getWorld()->getRB(i)->getName();
//            printf("Object %d: %s\n", i, sObjs.c_str());
//        }

        // Outer loop: Time
        int i = 0; // step counter
        double curr_time = 0.0;
        for (curr_time = 0.0; curr_time < total_time; curr_time += step_size) {
            if (simulator_->isBehaviorsDone()) {
                break; // Termination condition
            }

            // Step the simulator
            simulator_->runStep(step_size);
            // ... and update the step counter
            ++i;
//            if(i % steps_per_second == 0) 
//                printf("Time: %d seconds\n", i/steps_per_second);
            if (i % steps_per_sample == 0) {
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
            if (visualize_ && i % steps_per_visual == 0) {
                //cout << "Render " << i << " current time " << curr_time << endl;
                if (visualizeCapsuleCallback_) {
                    CartWheel::CapsuleState* capsule_state = new CapsuleState(simulator_);
                    visualizeCapsuleCallback_(*capsule_state);
                    delete capsule_state; // Ummm...
                }
                visualization_->render(simulator_);
                visualization_->glutStep();
            }
        }
        cout << "Simulation Time: " << curr_time << ", Steps: " << i << endl;

        // Reset the simulator for the next run
        simulator_->reset();
    }

    // Simulation terminates when all actors have run out of actions
    // "Dead space" in simulation is padded with standing still
    void SimulationInterface::simulate(vector<StartStatePtr> const &start_state,
            vector<vector<ExtendedActionPtr> > const &actions) {
        vector<BoxStatePtr> no_boxes;
        simulate(start_state, no_boxes, actions);
    }

    const vector<RelationalState*>& SimulationInterface::getRelations() const {
        return relations_;
    }

    const vector<PosState*>& SimulationInterface::getPositions() const {
        return positions_;
    }

    const vector<CapsuleState*>& SimulationInterface::getCapsules() const {
        return capsules_;
    }

} // namespace CartWheel
