#pragma once

#include <Utils/Utils.h>

#include <Physics/PhysicsDll.h>
#include <Physics/RigidBody.h>
#include <Physics/ArticulatedRigidBody.h>
#include <Physics/ArticulatedFigure.h>

namespace CartWheel { namespace Physics {

/*--------------------------------------------------------------------------------------------------------------------------------------------*
 * This class implements a container for rigid bodies (both stand alone and articulated). It reads a .rbs file and interprets it.             *
 *--------------------------------------------------------------------------------------------------------------------------------------------*/
class PHYSICS_DECLSPEC World{
friend class RBSimulator;
friend class ConCompositionFramework;

private:
	static World* _instance;
	static void create();
	static void destroy();

protected:
	//this is a list of all the objects in the world
	DynamicArray<RigidBody*> objects;
	//keep a list of all the articulated rigid bodies in the world as well, because we need to make sure that the user 
	//only ever links through joints articulated bodies, not just simple rigid bodies
	DynamicArray<ArticulatedRigidBody*> ABs;
	//we will also keep a list of all the articulated figures in the world, because we will need to access their joints, etc.
	DynamicArray<ArticulatedFigure*> AFs;
	//we'll keep a list of all the joints in the world as well, for quick access
	DynamicArray<Joint*> jts;

	//this is a list of all the contact points
	DynamicArray<ContactPoint> contactPoints;

protected:
	//the constructor
	World(void);
	//the destructor
	virtual ~World(void);

	// Destroy the world, it becomes unusable, but everything is clean
	virtual void destroyWorld();

public:	
	inline static World& instance() {
		if( _instance == NULL ) create();	
		return *_instance;
	}

	// Destroy all the objects, but the world is still usable
	virtual void destroyAllObjects();

	/**
		This method is used to call the draw method of all the rigid bodies in the world
	*/
	void drawRBs(int flags = SHOW_MESH);

	/**
		This method renders all the rigid bodies as a set of vertices 
		and faces that will be appended to the passed OBJ file.

		vertexIdxOffset indicates the index of the first vertex for this object, this makes it possible to render
		multiple different meshes to the same OBJ file
		 
		Returns the number of vertices written to the file
	*/
	unsigned int renderRBsToObjFile(FILE* fp, unsigned int vertexIdxOffset);


	/**
		This method is used to return a pointer to the list of contact forces
	*/
	inline DynamicArray<ContactPoint>* getContactForces(){
		return &contactPoints;
	}

	/**
		This method is used to integrate the forward simulation in time.
	*/
	virtual void advanceInTime(double deltaT) = 0;

	/**
		This method reads a list of rigid bodies from the specified file.
	*/
	virtual void loadRBsFromFile(const char* fName, const char* sPath, const char* afName);

	/**
		This method adds one rigid body (articulated or not).
	*/
	virtual void addRigidBody( RigidBody* rigidBody_disown);

	/**
		This method adds one articulated body.
	*/
	virtual void addArticulatedRigidBody( ArticulatedRigidBody* rigidBody_disown );

	/**
		This method adds one articulated figure.
	*/
	virtual void addArticulatedFigure( ArticulatedFigure* articulatedFigure_disown );

	/**
	 * Add a joint.
	 */
	virtual void addJoint(ArticulatedFigure* articulatedFigure, Joint* jt);

	/**
	 * Remove the given joint.
	 */
	virtual bool removeJoint(Joint* jt);

	/**
		This method returns the reference to the first articulated rigid body with 
		its name and its articulated figure name, or NULL if it is not found
	*/
	ArticulatedRigidBody* getARBByName(const char* name, const char* articulatedFigureName = NULL);

	/**
		This method returns the reference to the first articulated rigid body with 
		its name and its articulated figure name, or NULL if it is not found
	*/
	ArticulatedRigidBody* getARBByName(const char* name, ArticulatedFigure* articulatedFigure);

	/**
		This method returns the reference to the first rigid body with the given name, or NULL if it is not found
	*/
	RigidBody* getRBByName(const char* name);
	
	/**
		Return a rigid body by its index
	*/
	RigidBody* getRB(int i);
	
	/**
		Return the number of rigid bodies in the world
	*/
	int getRBCount();

	/**
		This method is used to get the state of all the rigid body in this collection.
	*/
	void getState(DynamicArray<double>* state);

	/**
		This method is used to set the state of all the rigid body in this collection.
	*/
	void setState(DynamicArray<double>* state, int start = 0);

	/**
		This method returns the number of articulated figures in this collection.
	*/
	inline int getAFCount(){
		return AFs.size();
	}

	/**
		This method returns the a reference to the ith articulated body
	*/
	inline ArticulatedFigure* getAF(int i){
		if (i<0 || (unsigned int)i>=AFs.size())
			return NULL;
		return AFs[i];
	}

	/**
		This method is used to update the toWorld transformations for the rigid bodies in the world
	*/
//	inline void updateTransformations(){
//		//and now make sure that each rigid body's toWorld transformation is updated
//		for (unsigned int i=0;i<objects.size();i++)
//			objects[i]->updateToWorldTransformation();
//	}

	/**
		this method applies a force to a rigid body, at the specified point. The point is specified in local coordinates,
		and the force is also specified in local coordinates.
	*/
	virtual void applyRelForceTo(RigidBody* b, const CartWheel::Math::Vector3d& f, const CartWheel::Math::Point3d& p) = 0;

	/**
		this method applies a force to a rigid body, at the specified point. The point is specified in local coordinates,
		and the force is specified in world coordinates.
	*/
	virtual void applyForceTo(RigidBody* b, const CartWheel::Math::Vector3d& f, const CartWheel::Math::Point3d& p) = 0;

	/**
		this method applies a torque to a rigid body. The torque is specified in world coordinates.
	*/
	virtual void applyTorqueTo(RigidBody* b, const CartWheel::Math::Vector3d& t) = 0;

	/**
		run a testing method...
	*/
	virtual void testAdvanceInTime(double /*deltaT*/){}

	/**
		another test method for advancing the world forward in time...
	*/
	virtual void runTest(){}

};

}
}
