#include <MathLib/stdafx.h>
#include <MathLib/Sphere.h>

using namespace CartWheel;
using namespace CartWheel::Math;
using namespace CartWheel::Util;

Sphere::Sphere(void){
	this->pos = Point3d();
	this->radius = 0;
}

Sphere::Sphere(const Point3d& p, double r){
	this->pos = p;
	this->radius = r;
}

Sphere::~Sphere(void){
}

