#include <Core/HumanoidIKCharacter.h>

using namespace CartWheel;
using namespace CartWheel::Core;
using namespace CartWheel::Physics;
using namespace CartWheel::Math;
using namespace CartWheel::Util;

HumanoidIKCharacter::HumanoidIKCharacter(Character* bip){
	this->biped = bip;
	//populate the relevant data members
	rightArm = new HumanoidIKArm(bip, "rElbow", "rShoulder");
	leftArm = new HumanoidIKArm(bip, "lElbow", "lShoulder");
//	setRightArmTarget(Point3d(0.6, 1.3, 0.3));
//	leftArm->setIKOrientations(Point3d(0.6, 1.3, 0.3));
}

HumanoidIKCharacter::~HumanoidIKCharacter(void){
	delete rightArm;
	delete leftArm;
}

void HumanoidIKCharacter::setRightArmTarget(Point3d p){
	rightArm->setIKOrientations(p);
	leftArm->setIKOrientations(p);
}


