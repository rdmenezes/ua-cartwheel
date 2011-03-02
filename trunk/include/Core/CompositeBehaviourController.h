/*
 * CompositeBehaviourController.h
 *
 *  Created on: Feb 28, 2011
 *      Author: denis
 */

#ifndef COMPOSITEBEHAVIOURCONTROLLER_H_
#define COMPOSITEBEHAVIOURCONTROLLER_H_

#include <Core/BehaviourController.h>
#include <Core/IKVMCController.h>
#include <Core/WorldOracle.h>
#include <Core/IKVMCController.h>
#include <Core/Character.h>

namespace CartWheel { namespace Core {

class CompositeBehaviourController : public BehaviourController {
private:
	DynamicArray<BehaviourController*> controllers;

public:
	CompositeBehaviourController(Character* b, IKVMCController* llc, WorldOracle* w);
	~CompositeBehaviourController() {}

	virtual void loadFromFile(FILE * f);
};

}
}

#endif /* COMPOSITEBEHAVIOURCONTROLLER_H_ */
