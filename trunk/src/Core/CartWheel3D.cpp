#include <Core/CartWheel3D.h>
#include <GLUtils/GLUtils.h>
#include <Core/TurnController.h>
#include <Core/ActionCollectionPolicy.h>

//#include <gr/gr_2D_bounding_box.h>
//#include <i/i_image.h>

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
    _path(""), _world(&World::instance()), _oracle(NULL)
{
	_oracle = new WorldOracle();
	_oracle->initializeWorld(_world);
}

CartWheel3D::CartWheel3D(const string& dataPath) :
    _path(dataPath), _world(&World::instance()), _oracle(NULL)
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

void CartWheel3D::addHuman(const string& characterFile, const string& controllerFile, const Point3d& pos, double heading)
{
    string sFile = _path + characterFile;
    _world->loadRBsFromFile(sFile.c_str(), _path.c_str());
    Character* ch = getAFtoCharacter(_world->getAF(_world->getAFCount()-1));
    ch->setHeading(heading);
    ch->setPos(pos);

#if 1
   // IKVMCController* c = new IKVMCController(ch);
#else
    SimBiController* c = new SimBiController(ch);
#endif

    sFile = _path + controllerFile;
    //c->loadFromFile(sFile.c_str());

#ifdef _SIMBICON
	CompositeController* con = new CompositeController(ch, "data/controllers/bigBird/RL/HMV/compositeController.con");

	ActionCollectionPolicy* policy = new ActionCollectionPolicy(con);
	policy->loadActionsFromFile("data/controllers/bigBird/RL/HMV/actions");

	CompositeControllerState state;
	state.primaryControllerIndex = 3;
	state.secondaryControllerIndex = 4;
	state.interpValue = 0.1;
	state.synchronizeControllers = TRUE;

	// TODO: Hack for now
	SimBiControllerState controllerState;
	controllerState.stance = 1;
	controllerState.phi = 0;
	controllerState.FSMStateIndex = 0;
	controllerState.bodyGroundContact = FALSE;

	for(int i=0; i<12; i++)
		state.controllerStates.push_back(controllerState);

	con->setControllerState(state);
//	SimpleStyleParameters params;
//	params.applyStyleParameters(con);

	Human* human = new Human(ch, con, policy);
#else

#if 1

	CompositeController* con = new CompositeController(ch, _oracle, "data/controllers/bipV3/HMV/compositeController.con");
	ActionCollectionPolicy* policy = new ActionCollectionPolicy(con);
	policy->loadActionsFromFile("data/controllers/bipV3/HMV/actions");

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

	ostringstream ostr;
	ostr << "Human" << _humans.size() + 1;
	string name = ostr.str();

	Human* human = new Human(name, ch, con, policy);

	// Select the default controller
	human->applyAction(0);

#else

	BehaviourController* behaviour = new TurnController(ch, c, _oracle);

	Human* human = new Human(ch, c, behaviour, NULL);

	human->setHeading(heading);

#endif
/*
	BehaviourController* behaviour = new TurnController(ch, c, _oracle);
    Human* human = new Human(ch, c, behaviour, NULL);
   // human->setHeading(heading);
*/
#endif
    _humans.push_back(human);
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

//    if(_oracle != NULL)
//        delete _oracle;
//
//    _oracle = new WorldOracle();
//    _oracle->initializeWorld(_world);
}

#define _SIMBICON

void CartWheel3D::runStep(double dt)
{
	double desiredFrameRate = 29.97;
	double animationTimeToRealTimeRatio = 1.3;
	double simulationTime = 0;
	double maxRunningTime = 0.98 / desiredFrameRate;

	DynamicArray<ContactPoint>* contactPoints = _world->getContactForces();

	while (simulationTime / maxRunningTime < animationTimeToRealTimeRatio)
    {
		simulationTime += SimGlobals::dt;

#ifdef _SIMBICON
	    for (unsigned int i = 0; i < _humans.size(); i++)
	    {
			SimBiController* c = _humans[i]->getController();
			c->performPreTasks(dt, contactPoints);

/*
	    	CompositeController* controller = _humans[i]->getCompositeController();
	    	if (NULL != controller)
	    	{
	    		int actionIndex = getController(_humans[i]->getName());

	    		if (actionIndex != -1)
	    		{
					SimBiController* c = controller->getController(actionIndex);
					c->performPreTasks(dt, contactPoints);
    }
	    	}

	    }
*/
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

#else
    for (unsigned int i = 0; i < _humans.size(); i++)
    {
    	SimBiController* c = _humans[i]->getController();

        c->performPreTasks(dt, contactPoints);
    }
#endif

    _world->advanceInTime(dt);

	contactPoints = _world->getContactForces();

#ifdef _SIMBICON
    for (unsigned int i = 0; i < _humans.size(); i++)
    {
    	SimBiController* c = _humans[i]->getController();
    	c->performPostTasks(dt, contactPoints);

    	/*
    	CompositeController* controller = _humans[i]->getCompositeController();
    	if (NULL != controller)
    	{
    		int actionIndex = getController(_humans[i]->getName());

    		if (actionIndex != -1)
    		{
				SimBiController* c = controller->getController(actionIndex);

				c->performPostTasks(dt, contactPoints);
    		}
*/
    	}
    }

#else
    for (unsigned int i = 0; i < _humans.size(); i++)
    {
    	SimBiController* c = _humans[i]->getController();

        if (c->performPostTasks(dt, contactPoints))
        {
            Vector3d step = Vector3d(c->getStanceFootPos(), c->getSwingFootPos());
            step = c->getCharacterFrame().inverseRotate(step);
            Vector3d v = c->getV();
//            double phi = c->getPhase();
            //printf("Human %d -> step: %f %f %f. Vel: %f %f %f phi = %f\n", i+1, step.x, step.y, step.z, v.x, v.y, v.z, phi);
        }
    }
#endif
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

bool CartWheel3D::getHuman(std::string name, Human** human)
{
	bool foundMatch = false;

    for (unsigned int i = 0; i < _humans.size(); i++)
    {
    	if (name == _humans[i]->getName())
    	{
    		*human = _humans[i];
    		foundMatch = true;
    		break;
    	}
    }

    return foundMatch;
}

void CartWheel3D::setController(const std::string& name, int actionIndex)
{
	Human* human = NULL;

	if (getHuman(name, &human))
	{
		human->applyAction(actionIndex);
/*
		ActionCollectionPolicy* policy = dynamic_cast<ActionCollectionPolicy*>(human->getPolicy());
		policy->setActionIndex(actionIndex);
		policy->applyAction();
*/
	}
}

int CartWheel3D::getController(const std::string& name)
{
	int actionIndex = 0;
	Human* human = NULL;

	if (getHuman(name, &human))
	{
		ActionCollectionPolicy* policy = dynamic_cast<ActionCollectionPolicy*>(human->getPolicy());
		actionIndex = policy->getActionIndex();
	}

	return actionIndex;
}
