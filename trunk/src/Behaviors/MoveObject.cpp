#include "Behaviors/MoveObject.h"
#include <Core/CartWheel3D.h>

using namespace CartWheel;
using namespace CartWheel::Core;

MoveObject::MoveObject(CartWheel3D* cw, std::string objName, MoveObject_Params* params)
        : Behavior(cw, objName, params!=NULL ? params->startTime : 0, params!=NULL ? params->duration : 0) {
    vSpeed = params->speed;
    vOrientation = params->orientation;
    vPosition = params->position;
    vAngSpeed = params->angSpeed;
    sObjName = objName;
}

void MoveObject::onInit() {
    cw->getObjectByName(sObjName)->setCMPosition(vPosition);
    cw->getObjectByName(sObjName)->setAngularVelocity(vAngSpeed);
    cw->getObjectByName(sObjName)->setCMVelocity(vSpeed);
    cw->getObjectByName(sObjName)->setOrientation(vOrientation.x, Vector3d(1, 0, 0));
    cw->getObjectByName(sObjName)->setOrientation(vOrientation.y, Vector3d(0, 1, 0));
    cw->getObjectByName(sObjName)->setOrientation(vOrientation.z, Vector3d(0, 0, 1));
}

void MoveObject::runStep() {
}

void MoveObject::onFinish() {
}

