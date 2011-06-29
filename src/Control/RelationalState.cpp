#include "Control/RelationalState.h"

namespace CartWheel {

    using Math::Point3d;
    using Physics::ContactPoint;
    using Util::StringUtils;

    RelationalState::RelationalState(vector<Relation> & rs) :
    myRelations() {
        for (int x = 0; x < rs.size(); x++)
            myRelations.push_back(new Relation(rs[x]));
    }

    bool RelationalState::containsAll(RelationalState & r) {
        vector<Relation*>::const_iterator itr = r.myRelations.begin();
        for (; itr != r.myRelations.end(); itr++) {
            if (!contains(**itr))
                return false;
        }
        return true;
    }

    RelationalState::~RelationalState() {
        fullClear();
    }

    bool RelationalState::contains(const Relation & r) {
        for (int x = 0; x < myRelations.size(); x++) {
            if (r.equals(*(myRelations[x])))
                return true;
        }
        return false;

    }

    Vector3d RelationalState::findPlace(int x, PosState & last, CartWheel3D * cw) {
        if (x < cw->getHumanCount()) {
            vector<string> humanNames;
            cw->getHumanNames(humanNames);
            //TODO: change this to returning a vector, not a list!
            //list<string>::const_iterator itr = humanNames.begin();

            //for (int i = 0; itr != humanNames.end(); itr++, i++)
            //{
            //      if(i == x){
            //              string s = *itr;
            return cw->getHumanPosition(humanNames[x]);
            //      }
            // }
        } else {
            Point3d
            pp =
                    cw->getWorld()->getRBByName(last.getName(x).c_str())->getCMPosition();
            return Vector3d(pp.getX(), pp.getY(), pp.getZ());
        }
    }

    string RelationalState::findName(int x, PosState & last) {
        return last.getName(x);
        //if(x < cw->getHumanCount()){
        //  return cw->getHuman(x)->getName();
        //}
        //else{
        //   return ;
        //}

    }

    string RelationalState::toString() {
        string s = "[";
        for (int j = 0; j < myRelations.size(); j++) {
            s.append(myRelations[j]->toString());
            if (j < myRelations.size() - 1)
                s.append(",");
        }
        s.append("]");
        return s;
    }

    void RelationalState::fullClear() {
        for (int i = 0; i < myRelations.size(); i++)
            delete myRelations[i];
        myRelations.clear();
    }

    void RelationalState::reset(PosState & last, CartWheel3D * cw) {
        double atThresh = 1.0;
        double changeThresh = 0.0001;
        //get each human (just doing humans now
        fullClear();
        if (last.getNumVectors() == 0)
            return;
        int numThings = last.getNumVectors();
        //just binary relations for right now
        for (int i = 0; i < numThings; i++) {
            Vector3d pos1 = findPlace(i, last, cw);
            string n1 = findName(i, last);
            for (int j = 0; j < numThings; j++) {
                if (i == j)
                    continue;
                Vector3d pos2 = findPlace(j, last, cw);
                double dist = ControlUtils::eucDistance2d(pos1, pos2);
                string n2 = findName(j, last);
                double prevDist = ControlUtils::eucDistance2d(
                        *(last.getPosition(n1)), *(last.getPosition(n2)));

                //                      cout<<"rel check "<<dist<<"  "<<prevDist<<endl;
                if (dist < atThresh) {
                    addRelation(*(new Relation("At", n1, n2)));
                }

                if (fabs(dist - prevDist) > changeThresh) {
                    if (dist - prevDist < 0)
                        addRelation(*(new Relation("DistanceDecreasing", n1, n2)));
                    else
                        addRelation(*(new Relation("DistanceIncreasing", n1, n2)));
                } else {
                    addRelation(*(new Relation("DistanceConstant", n1, n2)));
                }

            }
        }

        //      typedef boost::shared_ptr<Relation> RelationPtr;

        DynamicArray<ContactPoint>* contactPoints = cw->getWorld()->getContactForces();

        for (std::vector<ContactPoint>::iterator iter = contactPoints->begin(); iter != contactPoints->end(); ++iter) {
            //              addRelation(  *( new Relation("Contact",
            //                                                                        iter->rb1->getName(),
            //                                                                        iter->rb2->getName()) )  );
            Relation contact("Contact",
                    StringUtils::split(iter->rb1->getName(), ' ')[0],
                    StringUtils::split(iter->rb2->getName(), ' ')[0]);
            if (!contains(contact))
                addRelation(*(new Relation("Contact",
                    StringUtils::split(iter->rb1->getName(), ' ')[0],
                    StringUtils::split(iter->rb2->getName(), ' ')[0])));

        }
    }

} // namespace CartWheel
