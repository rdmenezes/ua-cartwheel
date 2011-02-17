#pragma once

#include <Physics/PhysicsDll.h>
#include <Physics/Joint.h>

namespace CartWheel { namespace Physics {

/*======================================================================================================================================================================*
 * This class is used to implement a hinge joint - angular impulses that allow relative rotation between the parent and the child only around a given axis must be      *
 * computed.                                                                                                                                                            *
 *======================================================================================================================================================================*/
class Joint;
class PHYSICS_DECLSPEC HingeJoint : public Joint{
friend class ODEWorld;
private:
/**
	Quantities that do not change
*/
	//This joint only allows relative motion about axis a - stored in parent coordinates
	CartWheel::Math::Vector3d a;
	//keep track of the joint limits as well - min and max allowed angles around the rotation axis
	double minAngle;
	double maxAngle;

	//this variable, updated at every sim step indicates wether there is a need for a joint limit constraint or not
	bool needJointLimitConstraint;

public:
	HingeJoint() {}
	virtual ~HingeJoint(void);

	/**
		This method is used to pass in information regarding the rotation axes. The string that is passed in is expected to have
		been read from an input file.
	*/
	virtual void readAxes(const char* axes);

	/**
		Sets the axis
	*/
	void setAxis( const CartWheel::Math::Vector3d& axis ) {
		a = axis;
		a.toUnit();
	}

	const CartWheel::Math::Vector3d& getAxis() const { return a; }

	/**
		This method is used to pass information regarding the joint limits for a joint. The string that is passed in is expected to
		have been read from an input file.
	*/
	virtual void readJointLimits(const char* limits);

	/**
		Set the joint limits
	*/
	void setJointLimits( double minAngle, double maxAngle ) {
		this->minAngle = minAngle;
		this->maxAngle = maxAngle;
		useJointLimits = true;
	}

	/**
		This method is used to fix the joint angular constraint to correct for drift. This is done by changing
		the orientation of the child.
	*/
	virtual void fixAngularConstraint(const CartWheel::Math::Quaternion& qRel);

	/**
		Return the A rotation axis
	*/
	inline CartWheel::Math::Vector3d getRotAxisA(){return a;}

	/**
		Returns the type of the current joint
	*/
	virtual int getJointType(){return HINGE_JOINT;}

};

}
}
