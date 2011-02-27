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
		if (NULL != m_behaviour)
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

		IKVMCController* ikvmccon = (dynamic_cast<IKVMCController*>(controller));

		if (NULL != ikvmccon)
		{
			setBehaviour(ikvmccon->getBehaviour());

			// Maintain the current heading
			m_behaviour->requestHeading(heading);

			//m_behaviour->setVelocities();

			m_behaviour->conTransitionPlan();
			//m_behaviour->setDesiredHeading(heading);
		}
	}
}

}

}
