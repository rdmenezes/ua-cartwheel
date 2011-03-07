#ifndef HUMAN_H_
#define HUMAN_H_

#include <string>

#include <MathLib/Vector3d.h>
#include <MathLib/Quaternion.h>
#include <MathLib/Point3d.h>
#include <MathLib/TransformationMatrix.h>

#include <Core/BehaviourController.h>
#include <Core/SimBiController.h>
#include <Core/CompositeController.h>
#include <Core/Policy.h>

#include <Physics/CollisionDetectionPrimitive.h>
#include <Physics/CapsuleCDP.h>

namespace CartWheel
{

namespace Core
{

class Human
{
private:
	std::string m_name;
	Character* m_character;
	SimBiController* m_controller;
	BehaviourController* m_behaviour;
	Policy* m_policy;
	CompositeController* m_composite_controller;

	Human() {}

public:

	Human(const std::string& name, CartWheel::Core::Character* character,
			CartWheel::Core::SimBiController* controller,
			CartWheel::Core::BehaviourController* behaviour = NULL, CartWheel::Core::Policy* policy = NULL);

	Human(const std::string& name, CartWheel::Core::Character* character,
			CartWheel::Core::CompositeController* controller, CartWheel::Core::Policy* policy);

	virtual ~Human();

	inline CartWheel::Core::Character* getCharacter() { return m_character; }
	inline CartWheel::Core::SimBiController* getController() { return m_controller; }
	inline CartWheel::Core::CompositeController* getCompositeController() { return m_composite_controller; }
	inline CartWheel::Core::BehaviourController* getBehaviour() { return m_behaviour; }
	inline CartWheel::Core::Policy* getPolicy() { return m_policy; }
	inline std::string getName() { return m_name; }

	inline CartWheel::Math::Vector3d getPosition() { return m_character->getCOM(); }
	inline double getHeading() { return m_character->getHeadingAngle(); }
    inline CartWheel::Math::Vector3d getVelocity() { return m_character->getCOMVelocity(); }

	void setCharacter(CartWheel::Core::Character* character);
	void setController(CartWheel::Core::SimBiController* controller);
	void setBehaviour(CartWheel::Core::BehaviourController* behaviour);
	void setPolicy(CartWheel::Core::Policy* policy);
	void setPosition(const CartWheel::Math::Point3d& position);
	void setHeading(double angle);
    void setSpeed(double speed);
    void setStepWidth(double width);
    void setName(const std::string& name);

    /**
     * Initialize everything.
     */
    void init();

    /**
     * Apply new action based on the index as prescribed in ActionCollectionPolicy.
     */
    void applyAction(int actionIndex);
};

}

}

#endif /* HUMAN_H_ */
