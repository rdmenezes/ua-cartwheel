#pragma once

#ifndef CARTWHEEL_3D_H
#define CARTWHEEL_3D_H

#include <vector>
#include <string>

#include <MathLib/Vector3d.h>
#include <MathLib/Quaternion.h>
#include <MathLib/Point3d.h>
#include <MathLib/TransformationMatrix.h>

#include <Core/TurnController.h>
#include <Core/DuckController.h>
#include <Core/WorldOracle.h>
#include <Core/IKVMCController.h>
#include <Core/SimBiController.h>

#include <Physics/CollisionDetectionPrimitive.h>
#include <Physics/CapsuleCDP.h>

using std::string;
using std::vector;

using CartWheel::Core::IKVMCController;
using CartWheel::Core::SimBiController;
using CartWheel::Core::BehaviourController;
using CartWheel::Core::Character;
using CartWheel::Core::WorldOracle;
using CartWheel::Core::SimGlobals;

using CartWheel::Physics::World;
using CartWheel::Physics::ArticulatedFigure;
using CartWheel::Physics::RigidBody;

using CartWheel::Math::Point3d;
using CartWheel::Math::Vector3d;
using CartWheel::Math::Quaternion;

namespace CartWheel {

class CartWheel3D
{
protected:
    vector<IKVMCController*> _controllers;
    vector<Character*> _characters;

    string _path;

    World* _world;
    WorldOracle* _oracle;

    void loadBehaviourController(Character* ch, IKVMCController* c, double dHeading);
    Character* getAFtoCharacter(ArticulatedFigure* af);

public:
    CartWheel3D() :
        _path(""), _world(&World::instance()), _oracle(NULL)
    {}

    CartWheel3D(const string& dataPath) :
        _path(dataPath), _world(&World::instance()), _oracle(NULL)
    {}

    ~CartWheel3D()
    {}

    /**
     * Add a human to the simulator.
     */
    void addHuman(const string& characterFile, const string& controllerFile, const Point3d& pos, double heading);
    void addObject(const string& name, const string& objFile, double mass);
    void addBox(const string& name, const Vector3d& scale, double mass);
    void addBall(const string& name, const Vector3d& scale, double mass);

    /**
     * Update the position, orientation, and velocity of the rigid body
     * indexed by the given name.
     */
    void updateRB(const string& name, const Point3d& pos, const Quaternion& orientation, const Vector3d& vel)
    {
        RigidBody* body = _world->getRBByName(name.c_str());
        body->setCMPosition(pos);
        body->setOrientation(orientation);
        body->setCMVelocity(vel);
    }

    /**********************************************************
     *                          GETTERS                       *
     **********************************************************/

    const string& getPath() const { return _path; }

    // TODO: This will probably give an error because it's not const.
    World* getWorld() { return _world; }

    Character* getHuman(int i) { return _characters[i]; }
    IKVMCController* getController(int i) { return _controllers[i]; }
    RigidBody* getObjectByName(const string& name) { return _world->getRBByName(name.c_str()); }
    int getHumanCount() { return _characters.size(); }

    Vector3d getHumanPosition(int i) { return _characters[i]->getCOM(); }
    double getHumanHeading(int i) { return _characters[i]->getHeadingAngle(); }
    Vector3d getHumanVelocity(int i) { return _characters[i]->getCOMVelocity(); }

    /**********************************************************
     *                          SETTERS                       *
     **********************************************************/

    void setPath(const string& path) { _path = path; }

    void setHumanPosition(int i, const Point3d& pos)
    {
        _world->getAF(i)->getRoot()->setCMPosition(pos);
    }

    void setHumanHeading(int i, double angle)
    {
        BehaviourController* b = _controllers[i]->getBehaviour();
        b->requestHeading(angle);
    }

    void setHumanSpeed(int i, double speed)
    {
        BehaviourController* b = _controllers[i]->getBehaviour();
        b->requestVelocities(speed, 0);
    }

    void setHumanStepWidth(int i, double width)
    {
        BehaviourController* b = _controllers[i]->getBehaviour();
        b->requestCoronalStepWidth(width);
    }

    void reset()
    {
        _controllers.clear();
        _characters.clear();

        _world->destroyAllObjects();

        //if(_oracle != NULL)
        //delete _oracle;
        //
        //_oracle = new WorldOracle();
        //_oracle->initializeWorld(_world);
    }

    void runStep(double dt = SimGlobals::dt);
};

} // namespace CartWheel3D

#endif // CARTWHEEL_3D_H

