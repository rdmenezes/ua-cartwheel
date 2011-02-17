#pragma once

#include <MathLib/Vector3d.h>

#include <Physics/PhysicsDll.h>

namespace CartWheel { namespace Physics {

class PHYSICS_DECLSPEC RBForceAccumulator{
public:
	//this is the total net force acting on a body
	CartWheel::Math::Vector3d netForce;
	//this is the total net torque (takes into account the torques due to forces as well)
	CartWheel::Math::Vector3d netTorque;
public:
	RBForceAccumulator(void);
	~RBForceAccumulator(void);
};

}
}
