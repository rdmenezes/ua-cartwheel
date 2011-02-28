#pragma once

#ifndef CARTWHEEL_3D_H
#define CARTWHEEL_3D_H

#include <vector>
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

	std::vector<Core::Human*> _humans;
    std::string _path;
    Physics::World* _world;
    Core::WorldOracle* _oracle;
    BuilderFunction _builderFunction;

    Core::Character* getAFtoCharacter(Physics::ArticulatedFigure* af);
    bool getHuman(std::string name, Core::Human** human);

public:

    CartWheel3D();
    CartWheel3D(const std::string& dataPath);

    ~CartWheel3D();

    /**
     * Add a human to the simulator.
     */
    void addHuman(const std::string& characterFile, const std::string& controllerFile, const Math::Point3d& pos, double heading);
    void addHuman(const std::string& name, const std::string& characterFile, const std::string& controllerFile, const std::string& actionFile,
    		const Math::Point3d& pos, double heading);

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

    Core::Human* getHuman(int i) { return _humans[i]; }
    Core::SimBiController* getController(int i) { return _humans[i]->getController(); }
    int getHumanCount() { return _humans.size(); }

    Math::Vector3d getHumanPosition(int i) { return _humans[i]->getPosition(); }
    double getHumanHeading(int i) { return _humans[i]->getHeading(); }
    Math::Vector3d getHumanVelocity(int i) { return _humans[i]->getVelocity(); }

    const std::string& getPath() const { return _path; }

    // TODO: This will probably give an error because it's not const.
    Physics::World* getWorld() { return _world; }

    Physics::RigidBody* getObjectByName(const std::string& name) { return _world->getRBByName(name.c_str()); }

    /*****************************o*****************************
     *                          SETTERS                       *
     **********************************************************/

    void setController(const std::string&, int actionIndex);
    int getController(const std::string& name);

    void setPath(const std::string& path) { _path = path; }

    void setHumanPosition(int i, const Math::Point3d& pos)
    {
        _world->getAF(i)->getRoot()->setCMPosition(pos);
    }

    void setHumanHeading(int i, double angle)
    {
    	_humans[i]->setHeading(angle);
    }

    void setHumanSpeed(int i, double speed)
    {
    	_humans[i]->setSpeed(speed);
    }

    void setHumanStepWidth(int i, double width)
    {
    	_humans[i]->setStepWidth(width);
    }

    void reset();

    void runStep(double dt = Core::SimGlobals::dt);

    void registerBuilder(BuilderFunction builderFunction)
    {
    	_builderFunction = builderFunction;
    }
};

} // namespace CartWheel3D

#endif // CARTWHEEL_3D_H

