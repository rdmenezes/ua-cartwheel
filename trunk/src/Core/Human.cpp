#include <Core/Human.h>
#include <Core/ActionCollectionPolicy.h>

using namespace CartWheel::Physics;
using namespace CartWheel::Math;
using namespace std;

namespace CartWheel
{
namespace Core
{

Human::Human(const std::string& name, Character* character,
		SimBiController* controller, BehaviourController* behaviour,
		Policy* policy) :
	m_name (name),
	m_character (character),
	m_controller (controller),
	m_behaviour (behaviour),
	m_policy (policy),
	m_composite_controller(NULL)
{
	if (NULL != m_behaviour)
	{
		IKVMCController* ikvmccon = (dynamic_cast<IKVMCController*>(controller));
		if (NULL != ikvmccon)
		{
			ikvmccon->setBehaviour(m_behaviour);
			m_behaviour->conTransitionPlan();
		}
	}
}

Human::Human(const std::string& name, Character* character,
		CompositeController* controller, Policy* policy) :
	m_name (name),
	m_character (character),
	m_controller (NULL),
	m_behaviour (NULL),
	m_policy (policy),
	m_composite_controller(controller)
{

}

Human::~Human()
{
	delete m_character;
	delete m_controller;
	delete m_behaviour;
	delete m_policy;
}

void Human::init()
{
	applyAction(0);
}

void Human::setCharacter(CartWheel::Core::Character* character)
{
	m_character = character;
}

void Human::setController(CartWheel::Core::SimBiController* controller)
{
	m_controller = controller;
}

void Human::setBehaviour(CartWheel::Core::BehaviourController* behaviour)
{
	m_behaviour = behaviour;
}

void Human::setPolicy(CartWheel::Core::Policy* policy)
{
	m_policy = policy;
}

void Human::setPosition(const CartWheel::Math::Point3d& position)
{
	m_character->setPos(position);
}

void Human::setHeading(double angle)
{
//	m_character->setHeading(angle);

	// If there is a behavior associated with the controller
	// ask it for a desired heading
	// If all else fails, set the heading on the controller directly.
	if (NULL != m_behaviour)
	{
		m_behaviour->requestHeading(angle);
		m_behaviour->conTransitionPlan();
	}
	else if (NULL != m_controller)
	{
		m_controller->setDesiredHeading(angle);
	}
}

void Human::setSpeed(double speed)
{
	if (NULL != m_behaviour)
	{
		m_behaviour->requestVelocities(speed, 0);
		m_behaviour->conTransitionPlan();
	}
}

void Human::setStepWidth(double width)
{
	if (NULL != m_behaviour)
	{
		m_behaviour->requestCoronalStepWidth(width);
		m_behaviour->conTransitionPlan();
	}
}

void Human::setName(const std::string& name)
{
	m_name = name;
}

void Human::applyAction(int actionIndex)
{
	ActionCollectionPolicy* policy = dynamic_cast<ActionCollectionPolicy*>(m_policy);

	if (NULL != policy)
	{
		double heading = getHeading();

		policy->setActionIndex(actionIndex);
		policy->applyAction();

		SimBiController* controller = m_composite_controller->getController(actionIndex);

		setController(controller);

		BehaviourController* behaviour = NULL;

		IKVMCController* ikvmccon = (dynamic_cast<IKVMCController*>(controller));

		if (NULL != ikvmccon)
		{
			behaviour = ikvmccon->getBehaviour();
		}

		setBehaviour(behaviour);

		// Maintain the current heading
		setHeading(heading);
	}
}

HingeJoint* g_Joint = NULL;

void Human::Drop(const std::string& targetName)
{
	(dynamic_cast<ODEWorld*>(&World::instance()))->removeJoint(g_Joint);
}

void Human::Grab(const std::string& targetName, const GrabbingMethod& method)
{
#ifdef TEST_JOINT
	Vector3d ballScale(1, 1, 1);
	double ballMass = 0.2;

	Point3d ballPosition(1, 1, -5);
	Vector3d ballVelocity(0, 0, 0);
	double yaw = 0;
	Quaternion ballOrientation(yaw, Vector3d(0, 1, 0));

    string mesh = _path + "data/models/sphereSmall.obj";
    Vector3d offset = Vector3d(0,0,0);

    ArticulatedRigidBody* body = new ArticulatedRigidBody();
    body->setName("pickup_ball");
  //  body->setScale(ballScale);
    body->addMeshObj(mesh.c_str(), offset, ballScale);

    body->setColour(0.1,0.1,0.9,1);
    body->setMass(ballMass);
    body->setMOI(Vector3d(0.2,0.2,0.2));

    Point3d center = Point3d(0,0,0);
    SphereCDP* sphereCDP = new SphereCDP(center, 0.1, body);
    body->addCollisionDetectionPrimitive(sphereCDP);

    body->setFrictionCoefficient(0.8);
    body->setRestitutionCoefficient(0.35);

    ArticulatedRigidBody* body1 = new ArticulatedRigidBody();
    body1->setName("pickup_ball1");
  //  body->setScale(ballScale);
    body1->addMeshObj(mesh.c_str(), offset, ballScale);

    body1->setColour(0.1,0.9,0.1,1);
    body1->setMass(ballMass);
    body1->setMOI(Vector3d(0.2,0.2,0.2));

    SphereCDP* sphereCDP1 = new SphereCDP(center, 0.1, body1);
    body1->addCollisionDetectionPrimitive(sphereCDP1);

    body1->setFrictionCoefficient(0.8);
    body1->setRestitutionCoefficient(0.35);

    Point3d ballPosition1(1.5, 1, -5);
    body1->setCMPosition(ballPosition1);

    body->setCMPosition(ballPosition);
    body->setOrientation(ballOrientation);
    body->setCMVelocity(ballVelocity);

    body->setParentJoint(NULL);

/*
    Point3d parentPos = lLowerArm->getCMPosition();
    Point3d childPos = parentPos;
//    childPos.x -= 0.05;

//	body->setCMPosition(childPos);

*/
	HingeJoint* tempJoint = new HingeJoint();
	tempJoint->setName("lHand2Ball");
    tempJoint->setAxis(Vector3d(0,1,0));
	tempJoint->setParentJointPosition(Point3d(0, 0, 0));
	tempJoint->setChildJointPosition(Point3d(-0.01, 0, 0));
	tempJoint->setJointLimits(-2.7, 0);

	tempJoint->setChild(body);
	tempJoint->setParent(body1);

    _world->addArticulatedRigidBody(body);
    _world->addArticulatedRigidBody(body1);

	ArticulatedFigure* figure = _world->getAF(_world->getAFCount()-1);

//	figure->addJoint(tempJoint);
//	figure->addArticulatedRigidBody(body);
	//figure->addArticulatedRigidBody(body1);

    //figure->fixJointConstraints();

    (dynamic_cast<ODEWorld*>(_world))->addJoint(figure, tempJoint);
#endif

	Vector3d ballScale(1, 1, 1);
	double ballMass = 0.0001;


	Point3d ballPosition(0, 0, 0);
	Vector3d ballVelocity(0, 0, 0);
	double yaw = 0;
	Quaternion ballOrientation(yaw, Vector3d(0, 1, 0));


    string mesh = "data/models/sphereSmall.obj";
    Vector3d offset = Vector3d(0,0,0);

    ArticulatedRigidBody* body = new ArticulatedRigidBody();
    body->setName("pickup_ball");
  //  body->setScale(ballScale);
    body->addMeshObj(mesh.c_str(), offset, ballScale);

    body->setColour(0.1,0.1,0.9,1);
    body->setMass(ballMass);
    body->setMOI(Vector3d(0.2,0.2,0.2));

    Point3d center = Point3d(0,0,0);
    SphereCDP* sphereCDP = new SphereCDP(center, 0.1, body);
    body->addCollisionDetectionPrimitive(sphereCDP);

    body->setFrictionCoefficient(0.8);
    body->setRestitutionCoefficient(0.35);


    body->setCMPosition(ballPosition);
    body->setOrientation(ballOrientation);
    body->setCMVelocity(ballVelocity);


    body->setParentJoint(NULL);

    World& world = World::instance();

	ArticulatedFigure* figure = world.getAF(world.getAFCount()-1);
    ArticulatedRigidBody* lLowerArm = figure->getARBByName("lLowerArm");

    Point3d parentPos = lLowerArm->getCMPosition();
    Point3d childPos = parentPos;
/*
    childPos.x += 0.1;
    childPos.y -= 0.05;
*/
   childPos.x -= 0.01;
   childPos.y -= 0.05;

	body->setCMPosition(childPos);

	HingeJoint* tempJoint = new HingeJoint();
	g_Joint = tempJoint;

	tempJoint->setName("lHand2Ball");
    tempJoint->setAxis(Vector3d(0,1,0));
	tempJoint->setParentJointPosition(Point3d(0.12, -0.05, 0));
	tempJoint->setChildJointPosition(Point3d(-0.01, 0, 0));
	tempJoint->setJointLimits(-2.7, 0);

	tempJoint->setChild(body);
	tempJoint->setParent(lLowerArm);

	figure->addJoint(tempJoint);
	figure->addArticulatedRigidBody(body);
    //figure->fixJointConstraints();

	world.addArticulatedRigidBody(body);

    (dynamic_cast<ODEWorld*>(&world))->addJoint(figure, tempJoint);
}

}
}
