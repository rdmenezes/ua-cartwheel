#ifndef _POSSTATE_H_
#define _POSSTATE_H_

#include <map>
#include <vector>
#include <string>
#include <Core/CartWheel3D.h>
#include <Control/BoxState.h>
#include <MathLib/Vector3d.h>
#include <sstream>

namespace CartWheel {

    class PosState {
    public:
        PosState();
        PosState(CartWheel3D * cw);
        const Math::Vector3d* getPosition(const std::string & n);
        void reset(CartWheel3D * cw);

        int getNumVectors() {
            return myNames.size();
        };

        std::string getName(int index) {
            return myNames[index];
        };

        Math::Vector3d getPosition(int index) {
            return myPositions[index];
        };

        const std::vector<BoxStatePtr>& getBoxStates() const;

    protected:
        std::map<std::string, int> blacklist;
        std::vector<Math::Vector3d> myPositions;
        std::vector<std::string> myNames;
        void populate(CartWheel3D * cw);
        std::vector<BoxStatePtr> box_states_;
    };

} // namespace CartWheel
#endif
