#include "Control/PosState.h"
#include <iostream>
#include <list>

using std::cout;
using std::endl;
using std::list;
using std::string;
using std::vector;
using std::stringstream;

namespace CartWheel {

    using Core::Human;
    using Math::Point3d;
    using Math::Vector3d;
    using Math::Quaternion;
    using Physics::RigidBody;

    PosState::PosState() {

    }

    PosState::PosState(CartWheel3D * cw) :
    blacklist() {
        Human* human = NULL;

        list<string> humanNames;
        bool result = cw->getHumanNames(humanNames);
        blacklist["ground"] = 1;
        blacklist["pelvis"] = 1;

        list<string>::const_iterator itr = humanNames.begin();
        for (int i = 0; itr != humanNames.end(); itr++, i++) {
            string name = (*itr);
            cw->getHuman(name, &human);

            stringstream ss;
            ss << "Human" << (i + 1) << " pelvis";
            blacklist[ss.str()] = 1; //TODO: Hacked this, not sure why the loop below doesn't fix it

            for (int j = 0; j < human->getCharacter()->getArticulatedRigidBodyCount(); j++) {
                blacklist[string(human->getCharacter()->getArticulatedRigidBody(j)->getName())] = 1;
            }
        }
        populate(cw);
    }

    void PosState::populate(CartWheel3D* cw) {
        vector<string> humanNames;
        bool result = cw->getHumanNames(humanNames);

        for (vector<string>::const_iterator itr = humanNames.begin(); itr != humanNames.end(); ++itr) {
            string name = (*itr);
            myNames.push_back(name);
            myPositions.push_back(cw->getHumanPosition(name));
        }

        // TODO: Later generalize to all objects, for now we only need boxes
        vector<string> boxes;
        cw->getBoxNames(boxes);
        for (vector<string>::iterator bx = boxes.begin(); bx != boxes.end(); ++bx) {
            string name = (*bx);
            RigidBody* rb = cw->getObjectByName(name);
            Point3d pos = rb->getCMPosition();
            Quaternion rot = rb->getOrientation();
            Vector3d size = rb->getScale();
            double mass = rb->getMass();
            BoxStatePtr box_state(new BoxState(name, pos, rot, size, mass));
            box_states_.push_back(box_state);
        }

        // This code was the old way of handling other objects (they were added to the same list as people)
        //  int others = cw->getWorld()->getRBCount();
        //  for (int j = 0; j < others; j++)
        //  {
        //    if (blacklist.count(cw->getWorld()->getRB(j)->getName()) == 0)
        //    {
        //      myNames.push_back(cw->getWorld()->getRB(j)->getName());
        //      Point3d pp = cw->getWorld()->getRB(j)->getCMPosition();
        //      myPositions.push_back(*(new Vector3d(pp.getX(), pp.getY(), pp.getZ())));
        //    }
        //  }
    }

    void PosState::reset(CartWheel3D * cw) {
        myNames.clear();
        myPositions.clear();
        populate(cw);
    }

    const Vector3d * PosState::getPosition(const string & n) {
        for (int i = 0; i < myNames.size(); i++) {
            if (n.compare(myNames[i]) == 0)
                return &(myPositions[i]);
        }
        return NULL;
    }

    const vector<BoxStatePtr>& PosState::getBoxStates() const {
        return box_states_;
    }

} // namespace CartWheel
