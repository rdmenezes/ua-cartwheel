#pragma once

#include <Utils/Observable.h>


#include <Physics/PhysicsDll.h>
#include <Physics/ArticulatedRigidBody.h>
#include <Physics/UniversalJoint.h>
#include <Physics/HingeJoint.h>
#include <Physics/BallInSocketJoint.h>

#include <iostream>
#include <sstream>

namespace CartWheel { namespace Physics {

/*======================================================================================================================================================================*
}
 * An articulated figure is composed of many articulated rigid bodies that are interconnected by joints. Characters, cars, ropes, etc, can all be viewed as articulated *
 * figures. One note is that we will only allow tree structures - no loops.                                                                                             *
 *======================================================================================================================================================================*/
class PHYSICS_DECLSPEC RBCollection;
class PHYSICS_DECLSPEC ArticulatedFigure : public CartWheel::Util::Observable {
friend class Character;
friend class ODEWorld;
friend class TestApp;
protected:
	//we will keep track of the root of the articulated figure. Based on the outgoing joints we can access its, children, and so on
	ArticulatedRigidBody* root;

	//this is the name of the articulated figure
	char name[100];
	double mass;
	static char prefixDelimiter;

	//keep a list of the character's joints, for easy access
	DynamicArray<Joint*> joints;

	DynamicArray<ArticulatedRigidBody*> arbs;


public:
	/**
		Default constructor
	*/
	ArticulatedFigure(void);

	/**
		Default destructor
	*/
	virtual ~ArticulatedFigure(void);

	void loadIntoWorld();

	/**
		Sets the root
	*/
	void setRoot( ArticulatedRigidBody* articulatedRigidBody_disown );

	/**
		returns the root of the current articulated figure.
	*/
	inline ArticulatedRigidBody* getRoot(){
		return root;
	}

	/**
		This method adds one rigid body (articulated or not).
	*/
	virtual void addArticulatedRigidBody( ArticulatedRigidBody* articulatedRigidBody_disown );

	/**
		This method removes one rigid body (articulated or not).
	*/
	virtual bool removeArticulatedRigidBody( ArticulatedRigidBody* articulatedRigidBody_disown );
	
	ArticulatedRigidBody* getArticulatedRigidBody( int i ) { return arbs[i]; }
	int getArticulatedRigidBodyCount() const { return arbs.size(); }

	void setName( const char* name ) {
		strncpy( this->name, name, 100 );
	}

	const char* getName() const {
		return name;
	}

	/**
	 * Add prefix to the current ArticulatedBody members
	 * so that they are unique in the world namespace.
	 * The prefix is separated from the original name by a space.
	 */
	void prefixARBNames(const char* prefixName);

	/**
	 * Strip away any prefixes from the passed in name.
	 * The prefix is assumed to be space separated from the name.
	 */
	static std::string getNoPrefixName(const std::string& name);

	/**
		This method returns an ARB that is a child of this articulated figure
	*/
	ArticulatedRigidBody* getARBByName(const char* name) const;

	/**
		Adds a joint to the figure
		This is an empty function as the joints are not tracked
		by the ArticulatedFigure.
		This makes it possible to disown Python of the joint pointer
		so that it doesn't garbage collect it. 
		The real place where Python should be disowned is when
		Joint.setParent() is called since the parent is responsible
		for deleting the joint. However, I don't know how to force
		python to disown an object when a method is called.
	*/
	void addJoint( Joint* jt );

	bool removeJoint( Joint* jt );

	/**
		This method is used to automatically fix the errors in the joints (i.e. drift errors caused by numerical integration). At some future
		point it can be changed into a proper stabilization technique.
	*/
	void fixJointConstraints(bool fixOrientations = true, bool fixVelocities = false);

	/**
		This method is used to get all the joints in the articulated figure and add them to the list of joints that is passed in as a parameter.
	*/
	//inline void addJointsToList(DynamicArray<Joint*> *joints);
	void addJointsToList(DynamicArray<Joint*> *joints);

	/**
		This method is used to compute the total mass of the articulated figure.
	*/
	void computeMass();

	/**
		This method is used to get the total mass of the articulated figure.
	*/
	double getMass();

	/**
		This method is used to load the details of an articulated figure from file. The PhysicalWorld parameter points to the world in which the objects
		that need to be linked live in.
	*/
	void loadFromFile(FILE* fp, World* world);

	/**
		Returns a pointer to the character's ith joint
	*/
	Joint* getJoint(int i){
		if (i < 0 || i > (int)joints.size()-1)
			return NULL;
		return joints[i];
	}

	/**
		This method is used to return the number of joints of the character.
	*/
	int getJointCount() {
		return joints.size();
	}

	/**
		this method is used to return a reference to the joint whose name is passed as a parameter, or NULL
		if it is not found.
	*/
	inline Joint* getJointByName(const char* jName){
		for (unsigned int i=0;i<joints.size();i++) {
			if (strcmp(joints[i]->name, jName) == 0) {
				return joints[i];
			}
		}
		return NULL;
	}

	/**
		this method is used to return the index of the joint (whose name is passed as a parameter) in the articulated figure hierarchy.
	*/
	inline int getJointIndex(const char* jName){
		for (unsigned int i=0;i<joints.size();i++)
			if (strcmp(joints[i]->name, jName) == 0)
				return i;
		return -1;
	}

	/**
		this method is used to return the index of the joint (whose name is passed as a parameter) in the articulated figure hierarchy.
	*/
	inline int getJointIndex(Joint* joint){
		for (unsigned int i=0;i<joints.size();i++)
			if (joints[i] == joint)
				return i;
		return -1;
	}
};

/**
	This method is used to get all the joints in the articulated figure and add them to the list of joints that is passed in as a paramter.
*/
//void addJointsToList(DynamicArray<Joint*> *joints);
//inline void addJointsToList(DynamicArray<Joint*> *joints){
//	if (!root)
//		return;
//	DynamicArray<ArticulatedRigidBody*> bodies;
//	bodies.push_back(root);
//
//	int currentBody = 0;
//
//	while ((unsigned int)currentBody < bodies.size()){
//		//add all the children joints to the list
//		for (unsigned int i=0; i < bodies[currentBody]->cJoints.size(); i++){
//			joints->push_back(bodies[currentBody]->cJoints[i]);
//			bodies.push_back(bodies[currentBody]->cJoints[i]->child);
//		}
//		currentBody++;
//	}
//}


/**
	This method is used to get all the joints in the articulated figure and add them to the list of joints that is passed in as a paramter.
*/
//inline void ArticulatedFigure::addJointsToList(DynamicArray<Joint*> *otherJoints){
//	for( unsigned int i=0; i<joints.size(); ++i )
//		otherJoints->push_back( joints[i] );
//}

}
}

//PHYSICS_TEMPLATE( DynamicArray<CartWheel::Physics::ArticulatedFigure*> )
