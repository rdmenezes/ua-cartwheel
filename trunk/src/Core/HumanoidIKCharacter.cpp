#include <Core/HumanoidIKCharacter.h>

using namespace CartWheel;
using namespace CartWheel::Core;
using namespace CartWheel::Physics;
using namespace CartWheel::Math;
using namespace CartWheel::Util;

HumanoidIKCharacter::HumanoidIKCharacter(Character* bip) {
    this->biped = bip;
    //populate the relevant data members
    leftArm = new HumanoidIKArm(bip, "lElbow", "lShoulder");
    rightArm = new HumanoidIKArm(bip, "rElbow", "rShoulder");
    spine = new HumanoidIKSpine(bip, "lowerback_torso", "pelvis_lowerback");
    leftLeg = new HumanoidIKLeg(bip, "lKnee", "lHip");
    rightLeg = new HumanoidIKLeg(bip, "rKnee", "rHip");
    //	setRightArmTarget(Point3d(0.6, 1.3, 0.3));
    //	leftArm->setIKOrientations(Point3d(0.6, 1.3, 0.3));
}

HumanoidIKCharacter::~HumanoidIKCharacter(void) {
    delete leftArm;
    delete rightArm;
    delete spine;
    delete leftLeg;
    delete rightLeg;
}

void HumanoidIKCharacter::setLeftArmTarget(Point3d p, Vector3d* shoulderAngles, double* elbowAngle) {
    Quaternion pOrientation, cOrientation;
    leftArm->setIKOrientations(p, &pOrientation, &cOrientation);

    //Left-Shoulder
    Quaternion qRot(0.707107, 0, 0, 0.707107);
    pOrientation = qRot*pOrientation;
    Vector3d vAngles = pOrientation.getAngles();
    double radX = vAngles.x;
    double radY = -vAngles.z;
    double radZ = vAngles.y;
    *shoulderAngles = Vector3d(radX, radY, radZ);

    //Left-Elbow
    *elbowAngle = -cOrientation.getAngles().y;
}

void HumanoidIKCharacter::setRightArmTarget(Point3d p, Vector3d* shoulderAngles, double* elbowAngle) {
    Quaternion pOrientation, cOrientation;
    rightArm->setIKOrientations(p, &pOrientation, &cOrientation);

    //Right-Shoulder
    Quaternion qRot(0.707107, 0, 0, 0.707107);
    pOrientation = qRot*pOrientation;
    Vector3d vAngles = pOrientation.getAngles();
    double radX = vAngles.y;
    double radY = -vAngles.z;
    double radZ = vAngles.x;
    *shoulderAngles = Vector3d(radX, radY, radZ);

    //Right-Elbow
    *elbowAngle = cOrientation.getAngles().y;
}

void HumanoidIKCharacter::setSpineTarget(Point3d p, double* leanS, double* leanC, double* twist) {
    Quaternion pOrientation, cOrientation;
    spine->setIKOrientations(p, &pOrientation, &cOrientation);

    Vector3d vAngles = pOrientation.getAngles();
    *leanS = vAngles.x*0.25;
    *leanC = -vAngles.y*0.25;
    *twist = vAngles.z;
}

void HumanoidIKCharacter::setLeftLegTarget(Point3d p) {
    Quaternion pOrientation, cOrientation;
    leftLeg->setIKOrientations(p, &pOrientation, &cOrientation);


    if(!isnan(pOrientation.getAngle())) {
        ReducedCharacterStateArray state;
        biped->getState(&state);
        ReducedCharacterState rs(&state);
        CartWheel::Math::Vector3d vh = pOrientation.getAngles();
        CartWheel::Math::Vector3d vk = pOrientation.getAngles();
//        printf("lHip: %f, %f, %f\n", vh.x, vh.y, vh.z);
        printf("lKnee: %f, %f, %f\n", vk.x, vk.y, vk.z);
        rs.setJointRelativeOrientation(cOrientation, 1);
        biped->setState(&state);
    } else {
        printf("Error IK leg target.\n");
    }
}

void HumanoidIKCharacter::setRightLegTarget(Point3d p) {
    Quaternion pOrientation, cOrientation;
    rightLeg->setIKOrientations(p, &pOrientation, &cOrientation);

    if(!isnan(pOrientation.getAngle())) {
        ReducedCharacterStateArray state;
        biped->getState(&state);
        ReducedCharacterState rs(&state);
        CartWheel::Math::Vector3d vh = pOrientation.getAngles();
        CartWheel::Math::Vector3d vk = pOrientation.getAngles();
//        printf("rHip: %f, %f, %f\n", vh.x, vh.y, vh.z);
        printf("rKnee: %f, %f, %f\n", vk.x, vk.y, vk.z);
        rs.setJointRelativeOrientation(cOrientation, 2);
        biped->setState(&state);
    } else {
        printf("Error IK leg target.\n");
    }
}
