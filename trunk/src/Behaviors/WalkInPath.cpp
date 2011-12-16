#include "Behaviors/WalkInPath.h"
#include <Core/CartWheel3D.h>

using namespace CartWheel;
using namespace CartWheel::GL;
using namespace CartWheel::Core;

WalkInPath::WalkInPath(CartWheel3D* cw, std::string humanName, WalkInPath_Params* params)
        : Behavior(cw, humanName, params!=NULL ? params->startTime : 0, params!=NULL ? params->duration : 0) {
    tPath = params->tPath;
    nAngSpeed = params->angSpeed;
    nStep = (nAngSpeed / abs(nAngSpeed))*0.5;

    this->cw = cw;
    this->cw->getHuman(humanName, &human);
    bcontroller = human->getBehaviour();
    nPoint = 1;
    Vector3d initPos = human->getPosition();
    tPath->addKnot(0, Point3d(initPos.x, initPos.y, initPos.z));

    for (int i = 0; i < tPath->getKnotCount(); i++) {
        double t = tPath->getKnotPosition(i);
        Point3d p = tPath->getKnotValue(i);
        printf("Knot %d: (time=%f) %f, %f, %f, Angle: %f\n", i, t, p.x, p.y, p.z, getAngle(p));
    }
}

void WalkInPath::onInit() {
}

void WalkInPath::runStep() {
    nSteps++;
    double dp = getDistance(nPoint);
    double tl = getTimeLeft(nPoint, time - startTime);
    double nSpeed = dp / tl;
//    nSpeed = nSpeed > 6 ? 6 : nSpeed;
//    nSpeed = nSpeed < 0 ? 0 : nSpeed;
    if (dp < 0.2 || tl <= 0) {
        nPoint++;
        if (nPoint >= tPath->getKnotCount())
            return;

        dp = getDistance(nPoint);
        tl = getTimeLeft(nPoint, time - startTime);
        nSpeed = dp / tl;
//        nSpeed = nSpeed > 6 ? 6 : nSpeed;
//        nSpeed = nSpeed < 0 ? 0 : nSpeed;
        double angle = getAngle(tPath->getKnotValue(nPoint));
        cw->setHumanHeading(humanName, angle);
        cw->setHumanSpeed(humanName, nSpeed);
    }
    //        Point3d pPath = getPathPoint(time - nStartTime);
    //        printf("%f, %f\n", pPath.x, pPath.z);
    if (nSteps >= SimGlobals::steps_per_second / 2) {// && dp > 1) {
        nSteps = 0;
        printf("Time: %f, Target-Point: %d, Distance: %f, Time Left: %f/%f, Speed: %f\n", time, nPoint, dp, tl, duration * tPath->getKnotPosition(nPoint), nSpeed);
        //            double angle = getAngle(pPath);
        //            printf("angle = %f\n", angle);
        double angle = getAngle(tPath->getKnotValue(nPoint));
        cw->setHumanHeading(humanName, angle);
        cw->setHumanSpeed(humanName, nSpeed);
    }
}

void WalkInPath::onFinish() {
}


double WalkInPath::getAngle(Point3d p3) {
    Vector3d p1 = human->getPosition();
    Point3d p2(p1.x, p1.y, p1.z + 1);
    Vector3d v1 = p2 - p1;
    Vector3d v2 = p3 - p1;
    double angle = atan2(v2.x, v2.z) - atan2(v1.x, v1.z);
    return angle;
}

Point3d WalkInPath::getPathPoint(double time) {
    double tc = time / duration;
    //    printf("tc=%f\n", tc);
    Point3d p = tPath->evaluate_linear(tc);
    return p;
}

double WalkInPath::getDistance(int nP) {
    Vector3d p1 = human->getPosition();
    Point3d p = tPath->getKnotValue(nP);
    double d = sqrt(pow(p1.x - p.x, 2) + pow(p1.z - p.z, 2));
    return d;
}

double WalkInPath::getTimeLeft(int nP, double time) {
    double ti = duration * tPath->getKnotPosition(nP);
    double tp = ti - time;
    return tp;
}