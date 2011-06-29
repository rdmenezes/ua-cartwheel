/*
 * SimulationInterface.h
 *
 *  Created on: Mar 1, 2011
 *      Author: dhewlett
 */

#ifndef SIMULATIONINTERFACE_H_
#define SIMULATIONINTERFACE_H_

#include <Core/CartWheel3D.h>
#include <Core/Visualization.h>
#include <Control/StaticWalkNearPolicy.h>
#include <Control/ExtendedAction.h>
#include <Control/RelationalState.h>
#include <Control/PosState.h>
#include <Control/CapsuleState.h>
#include <Control/RelationalState.h>
#include <Control/StartState.h>
#include <Control/BoxState.h>

#include <boost/shared_ptr.hpp>

namespace CartWheel {

    class SimulationInterface {
    private:

        // Making these private prevents copying, which results in segfaults during destruction, currently.
        //
        // We need to resolve this by either (1) implementing these two methods to do deep-copies of all pointers, (2) replace plointers with by-value members, or (3) using smart pointers.
        // --Kyle, March 12, 2011

        SimulationInterface(const SimulationInterface& other) {
        }

        SimulationInterface& operator=(const SimulationInterface& other) {
            return *this;
        }

    public:
        SimulationInterface(bool visualize);
        virtual ~SimulationInterface();

        // Daniel: This should really be private
        void init_simulation(std::vector<StartStatePtr> const& start_state, std::vector<BoxStatePtr> const& boxes);

        // The rest are supposed to be part of the public interface
        void simulate(std::vector<StartStatePtr> const& start_state, std::vector<std::vector<ExtendedActionPtr> > const& actions);
        void simulate(std::vector<StartStatePtr> const& start_state, std::vector<BoxStatePtr> const& boxes, std::vector<std::vector<ExtendedActionPtr> > const& actions);

        Visualization* getVisualization() {
            return visualization_;
        }
        const std::vector<PosState*>& getPositions() const;
        const std::vector<CapsuleState*>& getCapsules() const;
        const std::vector<RelationalState*>& getRelations() const;

        const std::vector<string>& getLastHumanNames() {
            return storedNames_;
        };

        /** 
         * Set a function to be called at each frame of simulation, which will receive
         * the scene's bounding capsules.  Useful for debugging
         */
        void setVisualizeCapsuleCallback(void (*cb)(const CartWheel::CapsuleState&)) {
            visualizeCapsuleCallback_ = cb;
        }

    private:
        CartWheel3D* simulator_;
        Visualization* visualization_;
        bool visualize_;
        std::string s_path_;
        void fullClear();

        std::vector<std::string> storedNames_;
        std::vector<RelationalState*> relations_;
        std::vector<PosState*> positions_;
        std::vector<CapsuleState*> capsules_;

        void (*visualizeCapsuleCallback_)(const CartWheel::CapsuleState&);
    };

} // namespace CartWheel
#endif /* SIMULATIONINTERFACE_H_ */
