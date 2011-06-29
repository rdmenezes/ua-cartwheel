/*
 * CapsuleState.cpp
 *
 *  Created on: Mar 1, 2011
 *      Author: dhewlett
 */

#include <Control/CapsuleState.h>
#include <Core/Human.h>
#include <list>

namespace CartWheel {
    using namespace std;
    using CartWheel::Core::Human;
    using CartWheel::Math::Capsule;
    using CartWheel::Math::Point3d;
    using CartWheel::Physics::CapsuleCDP;
    using CartWheel::Physics::ArticulatedRigidBody;

    CapsuleState::CapsuleState() {
    }

    CapsuleState::CapsuleState(CartWheel3D* cw) {
        populate(cw);
    }

    CapsuleState::~CapsuleState() {
        clear();
    }

    void CapsuleState::populate(CartWheel3D* cw) {
        Human* human = NULL;

        vector<string> humanNames;
        bool result = cw->getHumanNames(humanNames);

        vector<string>::iterator itr = humanNames.begin();
        for (; itr != humanNames.end(); itr++) {
            string name = (*itr);
            cw->getHuman(name, &human);

            names_.push_back(name);

            std::vector<Capsule*> capsules;
            for (int j = 0; j < human->getCharacter()->getArticulatedRigidBodyCount(); ++j) {
                ArticulatedRigidBody* bodyPart = human->getCharacter()->getArticulatedRigidBody(j);
                Point3d position = bodyPart->getCMPosition();
                CapsuleCDP* cdp = (CapsuleCDP*) (bodyPart->getCDP(0));

                Point3d p1 = bodyPart->getWorldCoordinates(cdp->getPoint1());
                Point3d p2 = bodyPart->getWorldCoordinates(cdp->getPoint2());

                capsules.push_back(new Capsule(p1, p2, cdp->getRadius()));
            }

            capsules_.push_back(capsules);
        }
    }

    int CapsuleState::getNumEntities() const {
        return names_.size();
    }

    const std::string& CapsuleState::getName(int index) const {
        return names_[index];
    }

    const std::vector<CartWheel::Math::Capsule*>& CapsuleState::getCapsules(int index) const {
        return capsules_[index];
    }

    void CapsuleState::clear() {
        for (int x = 0; x < capsules_.size(); x++)
            for (int y = 0; y < capsules_[x].size(); y++)
                delete capsules_[x][y];
        names_.clear();
        capsules_.clear();
    }

} // namespace CartWheel
