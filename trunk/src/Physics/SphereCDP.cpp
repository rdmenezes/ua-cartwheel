#include <Physics/CapsuleCDP.h>
#include <Physics/SphereCDP.h>
#include <Physics/PlaneCDP.h>
#include <Physics/RigidBody.h>

#include <GLUtils/GLUtils.h>
#include <MathLib/Segment.h>

using namespace CartWheel;
using namespace CartWheel::GL;
using namespace CartWheel::Physics;
using namespace CartWheel::Util;

SphereCDP::~SphereCDP(void){
}

/**
	Draw an outline of the sphere
*/
void SphereCDP::draw(){
	GLUtils::drawSphere(s.pos, s.radius, 5);
}

/**
	updates the world sphere.
*/
void SphereCDP::updateToWorldPrimitive(){
	wS.pos = bdy->getWorldCoordinates(s.pos);
	wS.radius = s.radius;
}

int SphereCDP::computeCollisionsWithPlaneCDP(PlaneCDP* p,  DynamicArray<ContactPoint> *cps){
	return getContactPoints(&this->wS, &p->wP, cps);
}

int SphereCDP::computeCollisionsWithCapsuleCDP(CapsuleCDP* c,  DynamicArray<ContactPoint> *cps){
	return getContactPoints(&this->wS, &c->wC, cps);
}

