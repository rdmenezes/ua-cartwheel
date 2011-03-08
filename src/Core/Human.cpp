#include <Core/Human.h>
#include <Core/ActionCollectionPolicy.h>

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
	//m_character->setHeading(angle);

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

}
}
