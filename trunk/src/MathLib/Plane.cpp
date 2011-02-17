#include <MathLib/stdafx.h>
#include <MathLib/Plane.h>

using namespace CartWheel;
using namespace CartWheel::Math;
using namespace CartWheel::Util;

Plane::Plane(void){
}

Plane::Plane(const Point3d& p, const Vector3d& n){
	this->n = n;
	//assume the normals are unit vectors.
//	this->n.toUnit();
	this->p = p;
}

Plane::~Plane(void){
}

