#include <Core/PoseController.h>
#include <Core/ConUtils.h>
#include <MathLib/Quaternion.h>
#include <Utils/Utils.h>

#include <cmath>

using namespace CartWheel;
using namespace CartWheel::Core;
using namespace CartWheel::Physics;
using namespace CartWheel::Math;
using namespace CartWheel::Util;

PoseController::PoseController(Character* ch) : Controller(ch) {

    //copy the current state of the character into the desired pose - makes sure that it's the correct size
    ch->getState(&desiredPose);

    //set initial values
    ReducedCharacterState rs(&desiredPose);
    rs.setPosition(Vector3d());
    rs.setVelocity(Vector3d());
    rs.setOrientation(Quaternion(1, 0, 0, 0));
    rs.setAngularVelocity(Vector3d());

    for (int i = 0; i < jointCount; i++) {
        controlParams.push_back(ControlParams(ch->getJoint(i)));
        rs.setJointRelativeAngVelocity(Vector3d(), i);
        rs.setJointRelativeOrientation(Quaternion(), i);
    }
}

PoseController::~PoseController(void) {
}

/**
        This method is used to compute the PD torque that aligns a child coordinate frame to a parent coordinate frame.
        Given: the current relative orientation of two coordinate frames (child and parent), the relative angular velocity,
        the desired values for the relative orientation and ang. vel, as well as the virtual motor's PD gains. The torque 
        returned is expressed in the coordinate frame of the 'parent'.
 */
Vector3d PoseController::computePDTorque(const Quaternion& qRel, const Quaternion& qRelD, const Vector3d& wRel, const Vector3d& wRelD, ControlParams* cParams) {
    Vector3d torque;
    //the torque will have the form:
    // T = kp*D(qRelD, qRel) + kd * (wRelD - wRel)

    //Note: There can be problems computing the proper torque from the quaternion part, because q and -q 
    //represent the same orientation. To make sure that we get the correct answer, we'll take into account
    //the sign of the scalar part of qErr - both this and the v part will change signs in the same way if either 
    //or both of qRel and qRelD are negative
    //	Quaternion qErr = qRel.getComplexConjugate() * qRelD;
    Quaternion qErr = qRel.getComplexConjugate();
    qErr *= qRelD;

    //qErr.v also contains information regarding the axis of rotation and the angle (sin(theta)), but I want to scale it by theta instead
    double sinTheta = qErr.v.length();
    if (sinTheta > 1)
        sinTheta = 1;
    if (IS_ZERO(sinTheta)) {
        //avoid the divide by close-to-zero. The orientations match, so the proportional component of the torque should be 0
    } else {
        double absAngle = 2 * asin(sinTheta);
        torque = qErr.v;
        torque *= 1 / sinTheta * absAngle * (-cParams->kp) * SGN(qErr.s);
        //		torque = qErr.v/sinTheta * absAngle * (-cParams->kp) * SGN(qErr.s);
    }

    //qErr represents the rotation from the desired child frame to the actual child frame, which
    //means that the torque is now expressed in child coordinates. We need to express it in parent coordinates!
    torque = qRel.rotate(torque);
    //the angular velocities are stored in parent coordinates, so it is ok to add this term now
    torque += (wRelD - wRel) * (-cParams->kd);
    torque *= cParams->strength;

    //now the torque is stored in parent coordinates - we need to scale it and apply torque limits
    scaleAndLimitTorque(&torque, cParams, qRel.getComplexConjugate());

    //and we're done...
    return torque;
}

/**
        This method is used to apply joint limits to the torque passed in as a parameter. It is assumed that
        the torque is already represented in the correct coordinate frame
 */
void PoseController::limitTorque(Vector3d* torque, ControlParams* cParams) {
    if (torque->x < -cParams->scale.x * cParams->maxAbsTorque) torque->x = -cParams->scale.x * cParams->maxAbsTorque;
    if (torque->x > cParams->scale.x * cParams->maxAbsTorque) torque->x = cParams->scale.x * cParams->maxAbsTorque;
    if (torque->y < -cParams->scale.y * cParams->maxAbsTorque) torque->y = -cParams->scale.y * cParams->maxAbsTorque;
    if (torque->y > cParams->scale.y * cParams->maxAbsTorque) torque->y = cParams->scale.y * cParams->maxAbsTorque;
    if (torque->z < -cParams->scale.z * cParams->maxAbsTorque) torque->z = -cParams->scale.z * cParams->maxAbsTorque;
    if (torque->z > cParams->scale.z * cParams->maxAbsTorque) torque->z = cParams->scale.z * cParams->maxAbsTorque;
}

/**
        This method is used to scale and apply joint limits to the torque that is passed in as a parameter. The orientation that transforms 
        the torque from the coordinate frame that it is currently stored in, to the coordinate frame of the 'child' to which the torque is 
        applied to (it wouldn't make sense to scale the torques in any other coordinate frame)  is also passed in as a parameter.
 */
void PoseController::scaleAndLimitTorque(Vector3d* torque, ControlParams* cParams, const Quaternion& qToChild) {
    //now change the torque to child coordinates
    *torque = qToChild.rotate(*torque);

    //and scale it differently along the main axis...
    torque->x *= cParams->scale.x;
    torque->y *= cParams->scale.y;
    torque->z *= cParams->scale.z;

    limitTorque(torque, cParams);

    // and now change it back to the original coordinates
    *torque = qToChild.getComplexConjugate().rotate(*torque);
}

/**
        This method is used to compute the torques that are to be applied at the next step.
 */
void PoseController::computeTorques(DynamicArray<ContactPoint>* /* cfs */) {
    
    Quaternion qRelD;
    Vector3d relAngVelD;

    Quaternion qRel;
    Vector3d wRel;

    ReducedCharacterState rs(&desiredPose);

    for (int i = 0; i < jointCount; i++) {
        if (controlParams[i].controlled == true) {

            RigidBody* parentRB = character->getJoint(i)->getParent();
            RigidBody* childRB = character->getJoint(i)->getChild();
            Quaternion parentQworld = parentRB->getOrientation().getComplexConjugate();

            Quaternion frameQworld;
            Vector3d frameAngularVelocityInFrame;

            if (controlParams[i].relToFrame == false) {
                frameQworld = parentQworld;
                frameAngularVelocityInFrame = parentQworld.rotate(parentRB->getAngularVelocity());
            } else {
                frameQworld = controlParams[i].frame.getComplexConjugate();
                frameAngularVelocityInFrame = frameQworld.rotate(controlParams[i].frameAngularVelocityInWorld);
            }

            Quaternion currentOrientationInFrame = frameQworld * childRB->getOrientation();
            Quaternion desiredOrientationInFrame = rs.getJointRelativeOrientation(i);
            Vector3d currentAngularVelocityInFrame = frameQworld.rotate(childRB->getAngularVelocity());
            Vector3d desiredRelativeAngularVelocityInFrame = rs.getJointRelativeAngVelocity(i);
            Vector3d currentRelativeAngularVelocityInFrame = currentAngularVelocityInFrame - frameAngularVelocityInFrame;

            Quaternion parentQframe = parentQworld * frameQworld.getComplexConjugate();

            torques[i] = computePDTorque(parentQframe * currentOrientationInFrame,
                    parentQframe * desiredOrientationInFrame,
                    parentQframe.rotate(currentRelativeAngularVelocityInFrame),
                    parentQframe.rotate(desiredRelativeAngularVelocityInFrame),
                    &controlParams[i]);
            torques[i] = parentRB->getWorldCoordinates(torques[i]);

            /*
                                    if (controlParams[i].relToCharFrame == false){
                                            //get the current relative orientation between the child and parent
                                            character->getRelativeOrientation(i, &qRel);
                                            //and the relative angular velocity, computed in parent coordinates
                                            character->getRelativeAngularVelocity(i, &wRel);
                                            //now compute the torque
                                            torques[i] = computePDTorque(qRel, rs.getJointRelativeOrientation(i), wRel, rs.getJointRelativeAngVelocity(i), &controlParams[i]);
                                            //the torque is expressed in parent coordinates, so we need to convert it to world coords now
                                            torques[i] = character->getJoint(i)->getParent()->getWorldCoordinates(torques[i]);
                                    }else{
                                            RigidBody* childRB = character->getJoint(i)->getChild();
                                            torques[i] = computePDTorque(childRB->getOrientation(), controlParams[i].charFrame * rs.getJointRelativeOrientation(i), childRB->getAngularVelocity(), rs.getJointRelativeAngVelocity(i), &controlParams[i]);
                                    }
             */

        } else {
            torques[i].setValues(0, 0, 0);
        }
    }

}

/**
        This method is used to parse the information passed in the string. This class knows how to read lines
        that have the name of a joint, followed by a list of the pertinent parameters. If this assumption is not held,
        then classes extended this one are required to provide their own implementation of this simple parser
 */
void PoseController::parseGainLine(const char* line) {
    double kp, kd, tMax, scX, scY, scZ;
    char jName[100];
    int jIndex;
    int nrParams = 0;
    nrParams = sscanf(line, "%s %lf %lf %lf %lf %lf %lf\n", jName, &kp, &kd, &tMax, &scX, &scY, &scZ);
    if (nrParams == 2) {
        Vector3d tmp = character->getJointByName(jName)->getChild()->getMOI();
        double maxM = std::max(tmp.x, tmp.y);
        maxM = std::max(maxM, tmp.z);
        kd = kp / 10;
        tMax = 10000;
        scX = tmp.x / maxM;
        scY = tmp.y / maxM;
        scZ = tmp.z / maxM;
    } else
        if (nrParams != 7)
        throwError("To specify the gains, you need: 'joint name Kp Kd Tmax scaleX scaleY scaleZ'! --> \'%s\'", line);
    jIndex = character->getJointIndex(jName);
    if (jIndex < 0)
        throwError("Cannot find joint: \'%s\'", jName);
    controlParams[jIndex].kp = kp;
    controlParams[jIndex].kd = kd;
    controlParams[jIndex].maxAbsTorque = tMax;
    controlParams[jIndex].scale = Vector3d(scX, scY, scZ);
}

/**
        This method is used to read the gain coefficients, as well as max torque allowed for each joint
        from the file that is passed in as a parameter.
 */
void PoseController::readGains(FILE* f) {
    if (f == NULL)
        throwError("File pointer is NULL - cannot read gain coefficients!!");

    //have a temporary buffer used to read the file line by line...
    char buffer[200];

    //this is where it happens.
    while (!feof(f)) {
        //get a line from the file...
        fgets(buffer, 200, f);
        if (strlen(buffer) > 195)
            throwError("The input file contains a line that is longer than ~200 characters - not allowed");
        char *line = lTrim(buffer);
        int lineType = getConLineType(line);
        switch (lineType) {
            case CON_PD_GAINS_END:
                return;
                break;
            case CON_PD_GAINS_START:
                break;
            case CON_COMMENT:
                break;
            default:
                parseGainLine(line);
                break;
        }
    }
    throwError("Incorrect controller input file: No \'/KpKdMaxT\' found", buffer);
}

/**
        This method is used to write the gain coefficients, as well as max torque allowed for each joint
        from the file that is passed in as a parameter.
 */
void PoseController::writeGains(FILE* f) {

    for (unsigned int jIndex = 0; jIndex < controlParams.size(); ++jIndex) {

        Joint* joint = character->getJoint(jIndex);
        if (!joint) continue;
        const char* jName = joint->getName();

        fprintf(f, "    %s\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",
                jName,
                controlParams[jIndex].kp,
                controlParams[jIndex].kd,
                controlParams[jIndex].maxAbsTorque,
                controlParams[jIndex].scale.x,
                controlParams[jIndex].scale.y,
                controlParams[jIndex].scale.z);

    }

}

/**
        This method is used to read the gain coefficients, as well as max torque allowed for each joint
        from the file that is passed in as a parameter.
 */
void PoseController::readGains(const char* fName) {
    FILE* f = fopen(fName, "r");
    if (f == NULL)
        return;

    readGains(f);
    fclose(f);
}

/**
        sets the targets to match the current state of the character
 */
void PoseController::setTargetsFromState() {
    ReducedCharacterState rs(&desiredPose);
    Quaternion qTemp;
    for (int i = 0; i < jointCount; i++) {
        character->getRelativeOrientation(i, &qTemp);
        rs.setJointRelativeOrientation(qTemp, i);
    }
}

