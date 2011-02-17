#pragma once

#include <MathLib/Vector3d.h>

#include <Physics/PhysicsDll.h>

namespace CartWheel { namespace Physics {

/**
	This class is used as a container for all the constants that are pertinent for the physical simulations, the controllers, etc.
*/

class PHYSICS_DECLSPEC PhysicsGlobals {
public:
	//We will assume that the gravity is in the y-direction (this can easily be changed if need be), and this value gives its magnitude. 
	static double gravity;
	static CartWheel::Math::Vector3d up;

};

}
}
