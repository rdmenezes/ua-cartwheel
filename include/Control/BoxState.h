/*
 * BoxStartState.h
 *
 *  Created on: Apr 7, 2011
 *      Author: dhewlett
 */

#ifndef BOXSTARTSTATE_H_
#define BOXSTARTSTATE_H_

#include <boost/shared_ptr.hpp>
#include <Core/CartWheel3D.h>

namespace CartWheel {

    class BoxState;
    typedef boost::shared_ptr<BoxState> BoxStatePtr;

    class BoxState {
        //    return CartWheel::Math::Quaternion::getRotationQuaternion(rotation_, )

    public:
        // This constructor takes a rotation about the y axis as a double

        BoxState(std::string name, Math::Vector3d position, double rotation, Math::Vector3d size, double mass) :
        name_(name), position_(position), rotation_(rotation), size_(size), mass_(mass) {
            Math::Vector3d axis(0, 1, 0);
            axis.toUnit();
            orientation_ *= Math::Quaternion::getRotationQuaternion(rotation, axis);
        };
        // This constructor takes a full orientation as a quaternion

        BoxState(std::string name, Math::Vector3d position, Math::Quaternion orientation, Math::Vector3d size, double mass) :
        name_(name), position_(position), orientation_(orientation), size_(size), mass_(mass) {
        };
        virtual ~BoxState();

        const std::string& getName() const {
            return name_;
        };

        const Math::Vector3d& getPosition() const {
            return position_;
        };

        const Math::Vector3d& getSize() const {
            return size_;
        };

        double getMass() const {
            return mass_;
        };

        const Math::Quaternion& getQuaternion() const {
            return orientation_;
        };

        void addToWorld(CartWheel3D* simulator) {
            simulator->addBox(name_, size_, position_, rotation_, mass_);
        };

    private:
        std::string name_;
        Math::Vector3d position_;
        double rotation_;
        Math::Quaternion orientation_;
        Math::Vector3d size_;
        double mass_;

    };

}
#endif /* BOXSTARTSTATE_H_ */

