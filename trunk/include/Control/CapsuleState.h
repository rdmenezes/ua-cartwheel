/*
 * CapsuleState.h
 *
 *  Created on: Mar 1, 2011
 *      Author: dhewlett
 */

#ifndef CAPSULESTATE_H_
#define CAPSULESTATE_H_

#include <Core/CartWheel3D.h>
#include <vector>

namespace CartWheel {

    class CapsuleState {
    public:
        CapsuleState();
        CapsuleState(CartWheel3D* cw);
        virtual ~CapsuleState();

        int getNumEntities() const;
        const std::string& getName(int index) const;

        /// Assuming "index" is for human, and the returned vector is over body parts
        const std::vector<CartWheel::Math::Capsule*>& getCapsules(int index) const;

        void clear();

    protected:
        void populate(CartWheel3D* cw);

        std::vector<std::string> names_;
        std::vector<std::vector<CartWheel::Math::Capsule*> > capsules_;

    private:

        // making copy methods private to prevent copying, as it currently 
        // results in segfaults during destruction

        CapsuleState(const CapsuleState&) {
        }

        CapsuleState& operator=(const CapsuleState&) {
        }

    };
}
#endif /* CAPSULESTATE_H_ */
