#include "MathLib/stdafx.h"

#include "MathLib/Capsule.h"

using namespace CartWheel;
using namespace CartWheel::Math;
using namespace CartWheel::Util;

Capsule::Capsule(void){
	radius = 0;
}

Capsule::Capsule(const Point3d& p1, const Point3d& p2, double r){
	this->p1 = p1;
	this->p2 = p2;
	this->radius = r;
}

Capsule::~Capsule(void){
}
