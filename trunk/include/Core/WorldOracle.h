#pragma once

#include <MathLib/Sphere.h>
#include <Physics/World.h>

namespace CartWheel { namespace Core {

class WorldOracle{
private:
	DynamicArray<CartWheel::Math::Sphere> spheres;

public:
	WorldOracle(void);
	virtual ~WorldOracle(void);

	virtual double getWorldHeightAt(CartWheel::Math::Point3d worldLoc);
	virtual void initializeWorld(CartWheel::Physics::World *physicalWorld);

	virtual void draw();
};

}
}
