#include <Core/Human.h>
#include <Core/ActionCollectionPolicy.h>

using namespace CartWheel::GL;

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

void Human::dropObject(const std::string& targetName)
{
	World& world = World::instance();

	bool foundMatch = (m_grabJoints.find(targetName) != m_grabJoints.end());
	if (foundMatch)
	{
		Joint* joint = m_grabJoints[targetName];
		(dynamic_cast<ODEWorld*>(&world))->removeJoint(joint);

		m_character->removeJoint(joint);

		m_grabJoints.erase(m_grabJoints.find(targetName));
	}

	foundMatch = (m_grabbedBodies.find(targetName) != m_grabbedBodies.end());
	if (foundMatch)
	{
		ArticulatedRigidBody* body = m_grabbedBodies[targetName];
		body->setParentJoint(NULL);
		body->setAFParent(NULL);

		m_grabbedBodies.erase(m_grabbedBodies.find(targetName));
	}
}

void Human::throwObject(const std::string& targetName, const Vector3d& velocity)
{
	bool foundMatch = (m_grabbedBodies.find(targetName) != m_grabbedBodies.end());
	if (foundMatch)
	{
		m_grabbedBodies[targetName]->setCMVelocity(velocity);
	}
	dropObject(targetName);
}

void Human::grabObject(const std::string& targetName, const GrabbingMethod& method)
{
    World& world = World::instance();

	RigidBody* rigidBody = world.getRBByName(targetName.c_str());
	ArticulatedRigidBody* body = NULL;
	bool foundMatch = (m_grabbedBodies.find(targetName) != m_grabbedBodies.end());
	bool result = false;

	if (foundMatch == false)
	{
		m_grabbedBodies[targetName] = dynamic_cast<ArticulatedRigidBody*>(rigidBody);
		body = m_grabbedBodies[targetName];
		result = (NULL != body);
	}

	ArticulatedRigidBody* attachmentHand = NULL;

	if (result)
	{
		switch (method)
		{
		case left:
			attachmentHand = m_character->getARBByName("lLowerArm");
			break;
		case right:
			attachmentHand = m_character->getARBByName("rLowerArm");
			break;
		case both:
			break;
		}

		result = (NULL != attachmentHand);
	}

	if (result)
	{
		Point3d parentPos = attachmentHand->getCMPosition();
		Point3d childPos = parentPos;
		Point3d parentJointPos;
		Point3d childJointPos;

		// The local coordinate systems of the left and right limbs are rotated pi over y axis
		// Create offsets to make the hand cling to an object by the palm(s)
		switch (method)
		{
		case left:
			childPos.x -= 0.03;
			childPos.y -= 0.07;
			parentJointPos = Point3d(0.09, -0.04, 0);
			childJointPos = Point3d(0, 0, 0);
			break;
		case right:
			childPos.x += 0.03;
			childPos.y -= 0.07;
			parentJointPos = Point3d(-0.09, -0.04, 0);
			childJointPos = Point3d(0, 0, 0);
			break;
		case both:
			break;
		}

		body->setCMPosition(childPos);
		// Force updating the body's position
		// TODO: This is too crude. The parent must be in the proximity to the body, which we do not have good control over yet.
		// The side-effects from the global rigid body updates may not be desirable, we really should only be updating
		// the body we are grabbing and nothing else.
		(dynamic_cast<ODEWorld*>(&world))->setEngineStateFromRB();

		HingeJoint* tempJoint = new HingeJoint();

		ostringstream ostr;
		ostr << "hand2" << targetName;
		string name = ostr.str();

		tempJoint->setName(name.c_str());
		tempJoint->setAxis(Vector3d(0,1,0));

		tempJoint->setParentJointPosition(parentJointPos);
		tempJoint->setChildJointPosition(childJointPos);

		// Setting high stop angle < low stop angle, effectively disables them, making the joint stiffer?
		tempJoint->setJointLimits(-1.7, -2.7);

		tempJoint->setChild(body);
		tempJoint->setParent(attachmentHand);

		m_character->addJoint(tempJoint);
		body->setAFParent(m_character);

		(dynamic_cast<ODEWorld*>(&world))->addJoint(m_character, tempJoint);

		m_grabJoints[targetName] = tempJoint;
	}
}

}
}
