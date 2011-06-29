#include <Control/WalkAction.h>
#include <Control/ControlUtils.h>

namespace CartWheel {

    void WalkAction::executeSetup(CartWheel3D * cw) {
        //find the actor and set his speed
        cw->doSerialBehavior("Walk", actorName, &Behaviors::Walk_Params(0, myTime, 
                mySpeed, cw->getHumanHeading(actorName)));
//        cw->setHumanSpeed(actorName, mySpeed);
    }

    void WalkAction::setParams(std::vector<double> & params) {
        setTime(params[0]);
        setSpeed(params[1]);
    }

    double WalkAction::getPrior(std::vector<double> & params) {
        double timeM = 8.0;
        double timeS = 5.0;

        double speedM = 1.0;
        double speedS = 1.0;

        double timePD = ControlUtils::gaussianPD(timeM, timeS, params[0]);
        double speedPD = ControlUtils::gaussianPD(speedM, speedS, params[1]);

        return timePD * speedPD;
    }

    double WalkAction::getParam(size_t i) const {

        // defer to parent class
        if (i < Base::numParams()) return Base::getParam(i);

        i -= Base::numParams();

        if (i >= myNumParams()) {
            throw std::out_of_range("WalkAction::getParam -- index is out of range");
        }

        switch (i) {
            case 0:
                return mySpeed;
            default:
                abort(); // This indicates a bug 
                // e.g. added a parameter, but forgot to add a case for it.
        }
    }

    double& WalkAction::getParam(size_t i) {
        // defer to parent class
        if (i < Base::numParams()) return Base::getParam(i);

        i -= Base::numParams();

        if (i >= myNumParams()) {
            throw std::out_of_range("WalkAction::getParam -- index is out of range");
        }

        switch (i) {
            case 0:
                return mySpeed;
            default:
                abort(); // This indicates a bug 
                // e.g. added a parameter, but forgot to add a case for it.
        }
    }

    UnitType WalkAction::getParamUnits(size_t i) const {
        // defer to parent class
        if (i < Base::numParams()) return Base::getParamUnits(i);

        i -= Base::numParams();

        if (i >= myNumParams()) {
            throw std::out_of_range("WalkAction::getParamUnits -- index is out of range");
        }

        switch (i) {
            case 0:
                return VSPACIAL_UNIT; // velocity: mySpeed
            default:
                abort(); // This indicates a bug 
                // e.g. added a parameter, but forgot to add a case for it.
        }
    }

} // namespace CartWheel

