#include <Core/CartWheel3D.h>
#include <GLUtils/GLUtils.h>
#include <Core/TurnController.h>
#include <Core/ActionCollectionPolicy.h>
#include <Core/CompositeBehaviourController.h>

#include <iostream>
#include <sstream>

using namespace std;

using namespace CartWheel;
using namespace CartWheel::Core;
using namespace CartWheel::GL;
using namespace CartWheel::Physics;
using namespace CartWheel::Math;
using namespace CartWheel::Util;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 1
#endif

CartWheel3D::CartWheel3D() :
    _path(""), _world(&World::instance()), _oracle(NULL),
    _builderFunction(NULL)
{
	_oracle = new WorldOracle();
	_oracle->initializeWorld(_world);
}

CartWheel3D::CartWheel3D(const string& dataPath) :
    _path(dataPath), _world(&World::instance()), _oracle(NULL),
    _builderFunction(NULL)
{
	_oracle = new WorldOracle();
	_oracle->initializeWorld(_world);
}

CartWheel3D::~CartWheel3D()
{
	_humans.clear();

    delete _oracle;
    _world->destroyAllObjects();
}

void CartWheel3D::addHuman(const string& name, const string& characterFile, const string& controllerFile, const Point3d& pos, double heading)
{
	bool foundMatch = (_humans.find(name) != _humans.end());
	if (foundMatch)
	{
		ostringstream osError;
		osError << name << " already exists!";
		throwError(osError.str().c_str());
	}

    string sFile = _path + characterFile;
    _world->loadRBsFromFile(sFile.c_str(), _path.c_str(), name.c_str());
    Character* ch = getAFtoCharacter(_world->getAF(_world->getAFCount()-1));
    ch->setHeading(heading);
    ch->setPos(pos);

    IKVMCController* c = new IKVMCController(ch);

    sFile = _path + controllerFile;
    c->loadFromFile(sFile.c_str());

	BehaviourController* behaviour = new TurnController(ch, c, _oracle);

	Human* human = new Human(name, ch, c, behaviour);

	human->setHeading(heading);
	human->init();

    _humans[name] = human;
}

void CartWheel3D::addHuman(const string& name, const std::string& characterFile, const std::string& controllerFile, const std::string& actionFile,
		const Math::Point3d& pos, double heading)
{
	bool foundMatch = (_humans.find(name) != _humans.end());
	if (foundMatch)
	{
		ostringstream osError;
		osError << name << " already exists!";
		throwError(osError.str().c_str());
	}

	string sFile = _path + characterFile;
    _world->loadRBsFromFile(sFile.c_str(), _path.c_str(), name.c_str());
    Character* ch = getAFtoCharacter(_world->getAF(_world->getAFCount()-1));
    ch->setHeading(heading);
    ch->setPos(pos);

	CompositeController* con = new CompositeController(ch, _oracle, controllerFile.c_str());
	ActionCollectionPolicy* policy = new ActionCollectionPolicy(con);
	policy->loadActionsFromFile(actionFile.c_str());

	// TODO: Hack for now
	CompositeControllerState state;
	state.primaryControllerIndex = 0;
	state.secondaryControllerIndex = 1;
	state.interpValue = 0.1;
	state.synchronizeControllers = TRUE;

	SimBiControllerState controllerState;
	controllerState.stance = 1;
	controllerState.phi = 0;
	controllerState.FSMStateIndex = 0;
	controllerState.bodyGroundContact = FALSE;

	for(int i=0; i<2; i++)
		state.controllerStates.push_back(controllerState);

	//con->setControllerState(state);

	// Create a new human
	Human* human = new Human(name, ch, con, policy);

	// Initialize
	human->init();

	_humans[name] = human;
}

void CartWheel3D::addObject(const string& name, const string& objFile, double mass)
{
    string tmp = _path + objFile;
    FILE* f = fopen(tmp.c_str(), "r");
    if (f == NULL)
    {
        throwError("Could not open file: obj: %s completePath: %s", objFile.c_str(), tmp.c_str());
    }

    // TODO: weird read from the buffer here. Should validate
    // that it was ok to remove it.
    char buffer[200];
    fgets(buffer, 200, f);
    RigidBody* rb = new RigidBody();
    rb->loadFromFile(f, _path.c_str());
    rb->setName(name.c_str());

    // TODO: close file??

    if (mass > 0)
    {
        rb->setMass(mass);
    }

    _world->addRigidBody(rb);
}

void CartWheel3D::addBox(const string& name, const Vector3d& scale, double mass)
{
    string mesh = _path + "data/models/box3.obj";
    Vector3d offset = Vector3d(0,0,0);

    RigidBody* body = new RigidBody();
    body->setName(name.c_str());
    body->setScale(scale);
    body->addMeshObj(mesh.c_str(), offset, scale);
    body->setColour(0.8,0.8,0.8,1);
    body->setMass(mass);
    body->setMOI(Vector3d(0.2,0.2,0.2));

    Point3d pos1 = Point3d(-scale.x, -scale.y, -scale.z);
    Point3d pos2 = Point3d(scale.x, scale.y, scale.z);
    //BoxCDP* boxCDP = new BoxCDP(pos1, pos2, body);
    body->addCollisionDetectionPrimitive(new BoxCDP(pos1, pos2, body));

    body->setFrictionCoefficient(0.8);
    body->setRestitutionCoefficient(0.35);

    _world->addRigidBody(body);
}

void CartWheel3D::addBall(const string& name, const Vector3d& scale, double mass)
{
    string mesh = _path + "data/models/sphere10x.obj";
    Vector3d offset = Vector3d(0,0,0);

    RigidBody* body = new RigidBody();
    body->setName(name.c_str());
    body->setScale(scale);
    body->addMeshObj(mesh.c_str(), offset, scale);

    body->setColour(0.8,0,0,1);
    body->setMass(mass);
    body->setMOI(Vector3d(0.2,0.2,0.2));

    Point3d center = Point3d(0,0,0);
    SphereCDP* sphereCDP = new SphereCDP(center, 0.1, body);
    body->addCollisionDetectionPrimitive(sphereCDP);

    body->setFrictionCoefficient(1.8);
    body->setRestitutionCoefficient(0.35);

    _world->addRigidBody(body);
}

void CartWheel3D::updateRB(const string& name, const Point3d& pos, const Quaternion& orientation, const Vector3d& vel) 
{
    RigidBody* body = _world->getRBByName(name.c_str());
    body->setCMPosition(pos);
    body->setOrientation(orientation);
    body->setCMVelocity(vel);
}

void CartWheel3D::reset()
{
	_humans.clear();

    _world->destroyAllObjects();

    if (NULL != _builderFunction) {
    	_builderFunction(this);
    }

//    if(_oracle != NULL)
//        delete _oracle;
//
//    _oracle = new WorldOracle();
//    _oracle->initializeWorld(_world);
}

void CartWheel3D::runStep(double dt)
{
	const double desiredFrameRate = 29.97;
	const double animationTimeToRealTimeRatio = 1.3;
	const double maxRunningTime = 0.98 / desiredFrameRate;
	double simulationTime = 0;

	DynamicArray<ContactPoint>* contactPoints = _world->getContactForces();

	DynamicArray<Vector3d> humanPositions;

	while ((simulationTime / maxRunningTime) < animationTimeToRealTimeRatio)
    {
		simulationTime += SimGlobals::dt;

		for (HumanItr itr = _humans.begin(); itr != _humans.end(); itr++)
	    {
			Human* human = (*itr).second;
			SimBiController* c = human->getController();
			if (NULL != c)
			{
				c->performPreTasks(dt, contactPoints);

				// Save the current position
				humanPositions.push_back(human->getPosition());

				BehaviourController* b = human->getBehaviour();
				if (NULL != b)
				{
					if (b->shouldAbort())
					{

					}
				}
			}
	    }

/*
    for (unsigned int i = 0; i < _humans.size(); i++)
    {
    	CompositeController* controller = _humans[i]->getCompositeController();
    	if (NULL != controller)
    	{
    		controller->advanceInTime(dt, contactPoints);
    	}
    }
*/

		_world->advanceInTime(dt);

		contactPoints = _world->getContactForces();

		for (HumanItr itr = _humans.begin(); itr != _humans.end(); itr++)
		{
			Human* human = (*itr).second;
			SimBiController* c = human->getController();

			if (NULL != c)
			{
				c->performPostTasks(dt, contactPoints);

				// Save the current position
				humanPositions.push_back(human->getPosition());

				BehaviourController* b = human->getBehaviour();
				if (NULL != b)
				{
					if (b->shouldAbort())
					{

					}
				}
			}
		}
    }
}

Character* CartWheel3D::getAFtoCharacter(ArticulatedFigure* af)
{
    Character* ch = new Character();
    ch->setRoot(af->getRoot());

    for(int i=0; i<af->getJointCount(); i++)
    {
        ch->addJoint(af->getJoint(i));
        ch->addArticulatedRigidBody(af->getJoint(i)->getChild());
    }
    ch->computeMass();
    return ch;
}

Math::Vector3d CartWheel3D::getHumanPosition(const std::string& name)
{
	return _humans[name]->getPosition();
}

double CartWheel3D::getHumanHeading(const std::string& name)
{
	return _humans[name]->getHeading();
}

Math::Vector3d CartWheel3D::getHumanVelocity(const std::string& name)
{
	return _humans[name]->getVelocity();
}

bool CartWheel3D::getHuman(const std::string& name, Human** human)
{
	bool foundMatch = false;

	foundMatch = (_humans.find(name) != _humans.end());

	if (foundMatch)
	{
		*human = _humans[name];
	}

    return foundMatch;
}

int CartWheel3D::getController(const std::string& name)
{
	int actionIndex = 0;
	Human* human = _humans[name];

	ActionCollectionPolicy* policy = dynamic_cast<ActionCollectionPolicy*>(human->getPolicy());
	if (NULL != policy)
	{
		actionIndex = policy->getActionIndex();
	}

	return actionIndex;
}

bool CartWheel3D::getHumanNames(std::vector<std::string>& names)
{
	for (HumanItr itr = _humans.begin(); itr != _humans.end(); itr++)
	{
		names.push_back((*itr).first);
	}

	bool result = names.size() > 0;

	return result;
}

bool CartWheel3D::getHumanNames(std::list<std::string>& names)
{
	for (HumanItr itr = _humans.begin(); itr != _humans.end(); itr++)
	{
		names.push_back((*itr).first);
	}

	bool result = names.size() > 0;

	return result;
}

int CartWheel3D::getHumanCount()
{
	return _humans.size();
}

const std::string& CartWheel3D::getPath()
{
	return _path;
}

// TODO: This will probably give an error because it's not const.
Physics::World* CartWheel3D::getWorld()
{
	return _world;
}

Physics::RigidBody* CartWheel3D::getObjectByName(const std::string& name)
{
	return _world->getRBByName(name.c_str());
}

void CartWheel3D::setHumanPosition(const std::string& name, const Math::Point3d& pos)
{
	_humans[name]->setPosition(pos);

    //_world->getAF(i)->getRoot()->setCMPosition(pos);
}

void CartWheel3D::setHumanHeading(const std::string& name, double angle)
{
	_humans[name]->setHeading(angle);
}

void CartWheel3D::setHumanSpeed(const std::string& name, double speed)
{
	_humans[name]->setSpeed(speed);
}

void CartWheel3D::setHumanStepWidth(const std::string& name, double width)
{
	_humans[name]->setStepWidth(width);
}

void CartWheel3D::setController(const std::string& name, int actionIndex)
{
	_humans[name]->applyAction(actionIndex);
}
