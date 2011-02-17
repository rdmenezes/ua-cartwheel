#pragma once

#include <Physics/PhysicsDll.h>
#include <Physics/Joint.h>

namespace CartWheel { namespace Physics {

/*======================================================================================================================================================================*
 * This class is used to implement a universal joint - angular impulses that allow only two degrees of freedom between the parent and the child must be computed.       *
 *======================================================================================================================================================================*/

class PHYSICS_DECLSPEC UniversalJoint : public Joint{
friend class ODEWorld;
private:
	//This joint can only rotate about the vector a, that is stored in parent coordinates
	CartWheel::Math::Vector3d a;
	//or about vector b that is stored in child coordinates
	CartWheel::Math::Vector3d b;
	//and the min and max allowed angles (around a axis)
	double minAngleA, maxAngleA;
	//and around the b axis
	double minAngleB, maxAngleB;
	//keep track, at any point in the simulation, if we need to enforce the joint limits about either axis A or axis B
	bool needJointLimitConstraintA, needJointLimitConstraintB;

public:
	UniversalJoint() {}
	~UniversalJoint(void);

	/**
		This method is used to pass in information regarding the rotation axes. The string that is passed in is expected to have
		been read from an input file.
	*/
	virtual void readAxes(const char* axes);

	/**
		Sets the two axes
	*/
	void setAxes( const CartWheel::Math::Vector3d& parentAxis, const CartWheel::Math::Vector3d& childAxis ) {
		this->a = parentAxis;
		this->b = childAxis;
		this->a.toUnit();
		this->b.toUnit();
	}
	
	void setParentAxis( const CartWheel::Math::Vector3d& parentAxis ) {
		this->a = parentAxis;
		this->a.toUnit();
	}

	const CartWheel::Math::Vector3d& getParentAxis() const { return a; }

	void setChildAxis( const CartWheel::Math::Vector3d& childAxis ) {
		this->b = childAxis;
		this->b.toUnit();
	}

	const CartWheel::Math::Vector3d& getChildAxis() const { return b; }

	/**
		This method is used to pass information regarding the joint limits for a joint. The string that is passed in is expected to
		have been read from an input file.
	*/
	virtual void readJointLimits(const char* limits);

	/**
		Set the joint limits
	*/
	void setJointLimits( double minAngleParent, double maxAngleParent, 
						 double minAngleChild, double maxAngleChild ) {

		this->minAngleA = minAngleParent;
		this->maxAngleA = maxAngleParent;
		this->minAngleB = minAngleChild;
		this->maxAngleB = maxAngleChild;

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
		Return the B rotation axis
	*/
	inline CartWheel::Math::Vector3d getRotAxisB(){return b;}


	/**
		Returns the type of the current joint
	*/
	virtual int getJointType(){ return UNIVERSAL_JOINT;}


};

}
}
