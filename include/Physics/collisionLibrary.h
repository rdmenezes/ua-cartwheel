/**
	A bunch of useful collision detection methods for different primitives are implemented here
*/
#pragma once

#include <MathLib/MathLib.h>
#include <MathLib/Sphere.h>
#include <MathLib/Plane.h>
#include <MathLib/Capsule.h>
#include <MathLib/Segment.h>

#include <Physics/PhysicsDll.h>
#include <Physics/ContactPoint.h>

namespace CartWheel { namespace Physics {

#define RETURN_CONTACTS_REVERSE_ORDER(a, b, cps)	{					\
	int start = cps->size();											\
	int n = getContactPoints(b, a, cps);								\
	for (int i=0;i<n;i++)												\
		((*cps)[start+i]).n *= -1;										\
	return n;															\
}																		\

/**
	collision between two CartWheel::Math::Spheres
*/
PHYSICS_DECLSPEC inline int getContactPoints(CartWheel::Math::Sphere* a, CartWheel::Math::Sphere* b, DynamicArray<ContactPoint> *cps){
	//compute the distance between the origins of the two CartWheel::Math::Spheres. If they are closer than a.r + b.r, then they've collided
	double distSquared = SQR(b->pos.x - a->pos.x) + SQR(b->pos.y - a->pos.y) + SQR(b->pos.z - a->pos.z);
	if (distSquared <= SQR(a->radius + b->radius)){
		cps->push_back(ContactPoint());
		ContactPoint* lastPoint = &(*cps)[cps->size()-1];
		double dist = sqrt(distSquared);
		//set the penetration depth
		lastPoint->d = (a->radius + b->radius - dist);
		//set the normal (pointing from b into a)
		lastPoint->n.setToVectorBetween(b->pos, a->pos); lastPoint->n /= dist;
		lastPoint->cp.setToOffsetFromPoint(b->pos, lastPoint->n, b->radius - lastPoint->d/2.0);
		return 1;
	}
	return 0;
}

/**
	collision between a CartWheel::Math::Sphere and a CartWheel::Math::Plane
*/
PHYSICS_DECLSPEC inline int getContactPoints(CartWheel::Math::Sphere* a, CartWheel::Math::Plane* b, DynamicArray<ContactPoint> *cps){
	//compute the distance between the CartWheel::Math::Plane and the CartWheel::Math::Sphere's origin
	CartWheel::Math::Vector3d v;
	v.setToVectorBetween(b->p, a->pos);
	double dist = v.dotProductWith(b->n);
	if (dist <= a->radius){
		cps->push_back(ContactPoint());
		ContactPoint* lastPoint = &(*cps)[cps->size()-1];
		//set the penetration depth
		lastPoint->d = (a->radius - dist);
		//set the normal (pointing from b into a, and the normal of b must points towards a)
		lastPoint->n = b->n;
		lastPoint->cp.setToOffsetFromPoint(a->pos, lastPoint->n, -a->radius);
		return 1;
	}
	return 0;
}

/**
	collision between a CartWheel::Math::Plane and a CartWheel::Math::Sphere
*/
PHYSICS_DECLSPEC inline int getContactPoints(CartWheel::Math::Plane* a, CartWheel::Math::Sphere* b, DynamicArray<ContactPoint> *cps){
	RETURN_CONTACTS_REVERSE_ORDER(a, b, cps);
}

/**
	collision between a CartWheel::Math::Capsule and a CartWheel::Math::Plane
*/
PHYSICS_DECLSPEC inline int getContactPoints(CartWheel::Math::Capsule* a, CartWheel::Math::Plane* b, DynamicArray<ContactPoint> *cps){
	//we'll look at the CartWheel::Math::Spheres that start at the end points of the CartWheel::Math::Capsule, and see if they interesect the CartWheel::Math::Plane
	CartWheel::Math::Sphere tmpSphere(a->p1, a->radius);
	int n = getContactPoints(&tmpSphere, b, cps);
	tmpSphere.pos = a->p2;
	n += getContactPoints(&tmpSphere, b, cps);
	return n;
}

/**
	collision between a CartWheel::Math::Plane and a CartWheel::Math::Capsule
*/
PHYSICS_DECLSPEC inline int getContactPoints(CartWheel::Math::Plane* a, CartWheel::Math::Capsule* b, DynamicArray<ContactPoint> *cps){
	RETURN_CONTACTS_REVERSE_ORDER(a, b, cps);
}

/**
	collision between a CartWheel::Math::Capsule and a CartWheel::Math::Sphere
*/
PHYSICS_DECLSPEC inline int getContactPoints(CartWheel::Math::Capsule* a, CartWheel::Math::Sphere* b, DynamicArray<ContactPoint> *cps){
	//a CartWheel::Math::Capsule is really an infinite number of CartWheel::Math::Spheres, whose centers lie on a segment.
	//so we need to find the point on the segment that is closest to the center of the CartWheel::Math::Sphere,
	//and then we'll use the two CartWheel::Math::Spheres that
	CartWheel::Math::Segment s(a->p1, a->p2);
	CartWheel::Math::Sphere tmpSphere;
	s.getClosestPointTo(b->pos, &tmpSphere.pos);
	tmpSphere.radius = a->radius;
	return getContactPoints(&tmpSphere, b, cps);
}

/**
	collision between a CartWheel::Math::Sphere and a CartWheel::Math::Capsule
*/
PHYSICS_DECLSPEC inline int getContactPoints(CartWheel::Math::Sphere* a, CartWheel::Math::Capsule* b, DynamicArray<ContactPoint> *cps){
	RETURN_CONTACTS_REVERSE_ORDER(a, b, cps);
}

/**
	collision between two CartWheel::Math::Capsules
*/
PHYSICS_DECLSPEC inline int getContactPoints(CartWheel::Math::Capsule* a, CartWheel::Math::Capsule* b, DynamicArray<ContactPoint> *cps){
	//a CartWheel::Math::Capsule is really an infinite number of CartWheel::Math::Spheres, whose centers lie on a segment.
	//so we need to find the closest segment between the segments of the two CartWheel::Math::Capsules,
	//then we'll place some CartWheel::Math::Spheres at the end points of that segment, and get the collisions
	//of that the 
	CartWheel::Math::Segment s;

	CartWheel::Math::Segment(a->p1, a->p2).getShortestSegmentTo(CartWheel::Math::Segment(b->p1, b->p2), &s);
	
	CartWheel::Math::Sphere sphere_a(s.a, a->radius);
	CartWheel::Math::Sphere sphere_b(s.b, b->radius);

	return getContactPoints(&sphere_a, &sphere_b, cps);
}

}
}
