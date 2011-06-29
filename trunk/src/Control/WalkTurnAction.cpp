#include <Control/WalkTurnAction.h>

namespace CartWheel {

    void WalkTurnAction::executeSetup(CartWheel3D * cw) {
        //find the actor and set his speed
        cw->doSerialBehavior("Walk", actorName, &Behaviors::Walk_Params(0, 
                myTime, mySpeed, myHeading));
//        cw->setHumanSpeed(actorName, mySpeed);
//        cw->setHumanHeading(actorName, myHeading);
    }

    void WalkTurnAction::setParams(std::vector<double> & params) {
        setTime(params[0]);
        setSpeed(params[1]);
        setHeading(params[2]);
    }

    double WalkTurnAction::getParam(size_t i) const {

        // defer to parent class
        if (i < Base::numParams()) return Base::getParam(i);

        i -= Base::numParams();

        if (i >= myNumParams()) {
            throw std::out_of_range("WalkTurnAction::getParam -- index is out of range");
        }

        switch (i) {
            case 0:
                return myHeading;
            default:
                abort(); // This indicates a bug 
                // e.g. added a parameter, but forgot to add a case for it.
        }
    }

    double& WalkTurnAction::getParam(size_t i) {
        // defer to parent class
        if (i < Base::numParams()) return Base::getParam(i);

        i -= Base::numParams();

        if (i >= myNumParams()) {
            throw std::out_of_range("WalkTurnAction::getParam -- index is out of range");
        }

        switch (i) {
            case 0:
                return myHeading;
            default:
                abort(); // This indicates a bug 
                // e.g. added a parameter, but forgot to add a case for it.
        }
    }

    UnitType WalkTurnAction::getParamUnits(size_t i) const {
        // defer to parent class
        if (i < Base::numParams()) return Base::getParamUnits(i);

        i -= Base::numParams();

        if (i >= myNumParams()) {
            throw std::out_of_range("WalkTurnAction::getParamUnits -- index is out of range");
        }

        switch (i) {
            case 0:
                return ANGLE_UNIT; // radians
            default:
                abort(); // This indicates a bug 
                // e.g. added a parameter, but forgot to add a case for it.
        }
    }

} // namespace CartWheel

