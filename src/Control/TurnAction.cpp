#include <Control/TurnAction.h>

namespace CartWheel {

    void TurnAction::executeSetup(CartWheel3D * cw) {
        //find the actor and set his speed        
        cw->doSerialBehavior("Walk", actorName, &Behaviors::Walk_Params(0.0, 
                myTime, NULL, myChange + cw->getHumanHeading(actorName)));
//        cw->setHumanHeading(actorName, myChange + cw->getHumanHeading(actorName));
    }

    void TurnAction::setParams(std::vector<double> & params) {
        setTime(params[0]);
        myChange = params[1];
    }

    double TurnAction::getPrior(std::vector<double> & params) {
        double timeM = 8.0;
        double timeS = 5.0;

        double speedM = 1.0;
        double speedS = 1.0;

        //  ControlUtils::gaussianPD(timeM, timeS, params[0]);
        //  ControlUtils::gaussianPD(speedM, speedS, params[1]);
    }

    double TurnAction::getParam(size_t i) const {
        // defer to parent class
        if (i < Base::numParams()) return Base::getParam(i);

        i -= Base::numParams();

        if (i >= myNumParams()) {
            throw std::out_of_range("TurnAction::getParam -- index is out of range");
        }

        switch (i) {
            case 0:
                return myChange;
            default:
                abort(); // This indicates a bug 
                // e.g. added a parameter, but forgot to add a case for it.
        }
    }

    double& TurnAction::getParam(size_t i) {
        // defer to parent class
        if (i < Base::numParams()) return Base::getParam(i);

        i -= Base::numParams();

        if (i >= myNumParams()) {
            throw std::out_of_range("TurnAction::getParam -- index is out of range");
        }

        switch (i) {
            case 0:
                return myChange;
            default:
                abort(); // This indicates a bug 
                // e.g. added a parameter, but forgot to add a case for it.
        }
    }

    UnitType TurnAction::getParamUnits(size_t i) const {
        // defer to parent class
        if (i < Base::numParams()) return Base::getParamUnits(i);

        i -= Base::numParams();

        if (i >= myNumParams()) {
            throw std::out_of_range("TurnAction::getParamUnits -- index is out of range");
        }

        switch (i) {
            case 0:
                return VANGLE_UNIT; // angular velocity: myChange
            default:
                abort(); // This indicates a bug 
                // e.g. added a parameter, but forgot to add a case for it.
        }

    }

} // namespace CartWheel
