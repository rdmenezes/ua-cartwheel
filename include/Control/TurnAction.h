#ifndef _FAKETURNACTION_H_
#define _FAKETURNACTION_H_

#include <Control/ExtendedAction.h>

namespace CartWheel {

    class TurnAction : public ExtendedAction {
        typedef ExtendedAction Base;

    public:

        TurnAction() {
            myChange = 0.0;
        }

        TurnAction(double change) {
            myChange = change;
        }
        void executeSetup(CartWheel3D * cw);

        virtual void setParams(std::vector<double> & params);

        virtual double getPrior(std::vector<double> & params);

        virtual size_t numParams() const {
            return Base::numParams() + myNumParams();
        }

        virtual double getParam(size_t i) const;
        virtual double& getParam(size_t i);

        virtual UnitType getParamUnits(size_t i) const;


    private:

        size_t myNumParams() const {
            return 1;
        }

        double myChange;
    };

} // namespace CartWheel
#endif
