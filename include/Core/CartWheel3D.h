#pragma once

#ifndef CARTWHEEL_3D_H
#define CARTWHEEL_3D_H

#include <vector>
#include <list>
#include <map>
#include <string>

#include <MathLib/Vector3d.h>
#include <MathLib/Quaternion.h>
#include <MathLib/Point3d.h>
#include <MathLib/TransformationMatrix.h>

#include <Core/BehaviourController.h>
#include <Core/WorldOracle.h>
#include <Core/SimBiController.h>
#include <Core/SimBiController.h>
#include <Core/Policy.h>
#include <Core/Human.h>

#include <Physics/CollisionDetectionPrimitive.h>
#include <Physics/CapsuleCDP.h>

namespace CartWheel {


class CartWheel3D
{

private:

	typedef void (*BuilderFunction)(CartWheel3D*);

	typedef std::map<std::string, Core::Human*> Humans;
	typedef std::map<std::string, Core::Human*>::iterator HumanItr;

	Humans _humans;
    std::string _path;
    Physics::World* _world;
    Core::WorldOracle* _oracle;
    BuilderFunction _builderFunction;

    // Putting copy constructor and assignment operator in the "private" section
    // to make this class uncopyable until the memory problems are fixed.
    // In particular, need all heap-allocated memory to be copied, or  redesign to store them by value (preferred).
    // Currently, copying results in a segfault upon desctruction, due to double-free.
    CartWheel3D(const CartWheel3D& other){}
    CartWheel3D& operator=(const CartWheel3D& other){return *this;};

public:

    CartWheel3D();
    CartWheel3D(const std::string& dataPath);

    ~CartWheel3D();

    /**
     * Add a human to the simulator.
     *
     * @param heading The human's facing direction (in radians from z+ axis)
     */
    void addHuman(const std::string& name, const std::string& characterFile, const std::string& controllerFile,
    		const Math::Point3d& pos, double heading);

    /// Add human with initial heading (in radians/ Add human with initial heading (in radians/ Add human with initial heading (in radians/ Add human with initial heading (in radians)
    void addHuman(const std::string& name, const std::string& characterFile, const std::string& controllerFile,
    		const std::string& actionFile, const Math::Point3d& pos, double heading);

    void addObject(const std::string& name, const std::string& objFile, double mass);
    void addBox(const std::string& name, const Math::Vector3d& scale, double mass);
    void addBall(const std::string& name, const Math::Vector3d& scale, double mass);

    /**
     * Update the position, orientation, and velocity of the rigid body
     * indexed by the given name.
     */
    void updateRB(const std::string& name, const Math::Point3d& pos, const Math::Quaternion& orientation, const Math::Vector3d& vel);

    /**********************************************************
     *                          GETTERS                       *
     **********************************************************/

    bool getHuman(const std::string& name, Core::Human** human);
    bool getHumanNames(std::vector<std::string>& names);
    bool getHumanNames(std::list<std::string>& names);


    int getHumanCount();

    Math::Vector3d getHumanPosition(const std::string& name);

    /// Get heading (in radians)
    double getHumanHeading(const std::string& name);
    Math::Vector3d getHumanVelocity(const std::string& name);

    const std::string& getPath();

    // TODO: This will probably give an error because it's not const.
    Physics::World* getWorld();

    Physics::RigidBody* getObjectByName(const std::string& name);

    /*****************************o*****************************
     *                          SETTERS                       *
     **********************************************************/

    void setController(const std::string&, int actionIndex);
    int getController(const std::string& name);

    void setPath(const std::string& path) { _path = path; }

    void setHumanPosition(const std::string& name, const Math::Point3d& pos);

    /// Set heading (in radians)
    void setHumanHeading(const std::string& name, double angle);
    void setHumanSpeed(const std::string& name, double speed);
    void setHumanStepWidth(const std::string& name, double width);

    void makeHumanGrabObject(const std::string& name, const std::string& targetName, const Core::Human::GrabbingMethod& method);
    void makeHumanThrowObject(const std::string& name, const std::string& targetName, const Math::Vector3d& velocity);
    void makeHumanDropObject(const std::string& name, const std::string& targetName);

    void reset();

    void runStep(double dt = Core::SimGlobals::dt);

    void registerBuilder(BuilderFunction builderFunction)
    {
    	_builderFunction = builderFunction;
    }
};

} // namespace CartWheel3D

#endif // CARTWHEEL_3D_H

