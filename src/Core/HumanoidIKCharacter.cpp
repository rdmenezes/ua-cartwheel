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
    leftHand = new HumanoidIKHand(bip, "lHandJoint", "lElbow");
    rightHand = new HumanoidIKHand(bip, "rHandJoint", "rElbow");
    pelvisTorso = new HumanoidIKPelvisTorso(bip, "lowerback_torso", "pelvis_lowerback");
    head = new HumanoidIKHead(bip, "torso_head", "lowerback_torso");
    leftLeg = new HumanoidIKLeg(bip, "lKnee", "lHip");
    rightLeg = new HumanoidIKLeg(bip, "rKnee", "rHip");
    //	setRightArmTarget(Point3d(0.6, 1.3, 0.3));
    //	leftArm->setIKOrientations(Point3d(0.6, 1.3, 0.3));
}

HumanoidIKCharacter::~HumanoidIKCharacter(void) {
    delete leftArm;
    delete rightArm;
    delete pelvisTorso;
    delete leftLeg;
    delete rightLeg;
    delete head;
    delete leftHand;
    delete rightHand;
}

void HumanoidIKCharacter::setLeftArmTarget(Point3d p, Vector3d* shoulderAngles, Vector3d* elbowAngles) {
    Quaternion pOrientation, cOrientation;
    leftArm->setIKOrientations(p, &pOrientation, &cOrientation);

    //Left-Shoulder
    Vector3d vAngles = pOrientation.getAngles();
    double radX = vAngles.x;
    double radY = vAngles.y;
    double radZ = vAngles.z;
    *shoulderAngles = Vector3d(radX, radY, radZ);   
    

    //Left-Elbow
    vAngles = cOrientation.getAngles();
    radX = vAngles.x;
    radY = vAngles.y;
    radZ = vAngles.z;
    *elbowAngles = Vector3d(radX, radY, radZ);
    
    
//    printf("Shoulder = %f, %f, %f, %f\n", pOrientation.s, pOrientation.v.x, pOrientation.v.y, pOrientation.v.z);
//    ReducedCharacterStateArray state;
//    biped->getState(&state);
//    ReducedCharacterState rs(&state);
//    rs.setJointRelativeOrientation(pOrientation, biped->getJointIndex("lShoulder"));
//    rs.setJointRelativeOrientation(cOrientation, biped->getJointIndex("lElbow"));
//    biped->setState(&state);
}

void HumanoidIKCharacter::setRightArmTarget(Point3d p, Vector3d* shoulderAngles, Vector3d* elbowAngles) {
    Quaternion pOrientation, cOrientation;
    rightArm->setIKOrientations(p, &pOrientation, &cOrientation);

    //Right-Shoulder
    Vector3d vAngles = pOrientation.getAngles();
    double radX = vAngles.x;
    double radY = vAngles.y;
    double radZ = vAngles.z;
    *shoulderAngles = Vector3d(radX, radY, radZ);

    //Right-Elbow
    vAngles = cOrientation.getAngles();
    radX = vAngles.x;
    radY = vAngles.y;
    radZ = vAngles.z;
    *elbowAngles = Vector3d(radX, radY, radZ);
}

void HumanoidIKCharacter::setLeftHandTarget(Point3d p, Vector3d* handAngles) {
    Quaternion pOrientation, cOrientation;
    leftHand->setIKOrientations(p, &pOrientation, &cOrientation);

    //Left-Hand
    Vector3d vAngles = cOrientation.getAngles();
    double radX = vAngles.x;
    double radY = vAngles.y;
    double radZ = vAngles.z;
    *handAngles = Vector3d(radX, radY, radZ); 
}

void HumanoidIKCharacter::setRightHandTarget(Point3d p, Vector3d* handAngles) {
    Quaternion pOrientation, cOrientation;
    rightHand->setIKOrientations(p, &pOrientation, &cOrientation);

    //Right-Hand
    Vector3d vAngles = cOrientation.getAngles();
    double radX = vAngles.x;
    double radY = vAngles.y;
    double radZ = vAngles.z;
    *handAngles = Vector3d(radX, radY, radZ);
}

void HumanoidIKCharacter::setPelvisTorsoTarget(Point3d p, Vector3d* pelvisAngles, Vector3d* torsoAngles) {
    Quaternion pOrientation, cOrientation;
    pelvisTorso->setIKOrientations(p, &pOrientation, &cOrientation);

    Vector3d vAngles = pOrientation.getAngles();
    double radX = vAngles.x;
    double radY = vAngles.y;
    double radZ = vAngles.z;
    *pelvisAngles = Vector3d(radX, radY, radZ);

    vAngles = cOrientation.getAngles();
    radX = vAngles.x;
    radY = vAngles.y;
    radZ = vAngles.z;
    *torsoAngles = Vector3d(radX, radY, radZ);
}

void HumanoidIKCharacter::setHeadTarget(Point3d p, Vector3d* headAngles) {
    Quaternion pOrientation, cOrientation;
    head->setIKOrientations(p, &pOrientation, &cOrientation);

    Vector3d vAngles = cOrientation.getAngles();
    double radX = vAngles.x;
    double radY = vAngles.y;
    double radZ = vAngles.z;
    *headAngles = Vector3d(radX, radY, radZ);
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


//Trying to make kind of Forward Kinematics
////    Character* ch = new Character(dynamic_cast<ArticulatedFigure*>(biped));
////    Point3d handPos = ch->getARBByName("lHand")->getCMPosition();
////    printf("lHand before IK: %f, %f, %f\n", handPos.x, handPos.y, handPos.z);
////    ReducedCharacterStateArray state;
////    ch->getState(&state);
////    ReducedCharacterState rs(&state);
////    rs.setJointRelativeOrientation(pOrientation, ch->getJointIndex("lShoulder"));
////    rs.setJointRelativeOrientation(cOrientation, ch->getJointIndex("lElbow"));
////    ch->setState(&state);
////    handPos = ch->getARBByName("lHand")->getCMPosition();
////    printf("lHand after IK: %f, %f, %f\n", handPos.x, handPos.y, handPos.z);