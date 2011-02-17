#include <Core/CartWheel3D.h>
#include <GLUtils/GLUtils.h>
//#include <gr/gr_2D_bounding_box.h>
//#include <i/i_image.h>

#include <iostream>

using std::cout;
using std::endl;

using namespace CartWheel;
using namespace CartWheel::Core;
using namespace CartWheel::GL;
using namespace CartWheel::Physics;
using namespace CartWheel::Math;
using namespace CartWheel::Util;

void CartWheel3D::addHuman(const string& characterFile, const string& controllerFile, const Point3d& pos, double heading)
{
    string sFile = _path + characterFile;
    _world->loadRBsFromFile(sFile.c_str(), _path.c_str());
    Character* ch = getAFtoCharacter(_world->getAF(_world->getAFCount()-1));
    ch->setHeading(heading);
    ch->setPos(pos);

    IKVMCController* c = new IKVMCController(ch);
    sFile = _path + controllerFile;
    c->loadFromFile(sFile.c_str());

    _controllers.push_back(c);
    _characters.push_back(ch);

    loadBehaviourController(ch, c, heading);
}

void CartWheel3D::addObject(const string& name, const string& objFile, double mass)
{
    string tmp = _path + objFile;
    FILE* f = fopen(tmp.c_str(), "r");
    if (f == NULL)
    {
        throwError("Could not open file: obj: %s completePath: %s", objFile.c_str(), tmp.c_str());
    }

    // TODO: weird read from the buffer here. Should validate
    // that it was ok to remove it.
    char buffer[200];
    fgets(buffer, 200, f);
    RigidBody* rb = new RigidBody();
    rb->loadFromFile(f, _path.c_str());
    rb->setName(name.c_str());

    // TODO: close file??

    if (mass > 0)
    {
        rb->setMass(mass);
    }

    _world->addRigidBody(rb);
}

void CartWheel3D::addBox(const string& name, const Vector3d& scale, double mass)
{
    string mesh = _path + "data/models/box3.obj";
    Vector3d offset = Vector3d(0,0,0);

    RigidBody* body = new RigidBody();
    body->setName(name.c_str());
    body->setScale(scale);
    body->addMeshObj(mesh.c_str(), offset, scale);
    body->setColour(0.8,0.8,0.8,1);
    body->setMass(mass);
    body->setMOI(Vector3d(0.2,0.2,0.2));

    Point3d pos1 = Point3d(-scale.x, -scale.y, -scale.z);
    Point3d pos2 = Point3d(scale.x, scale.y, scale.z);
    //BoxCDP* boxCDP = new BoxCDP(pos1, pos2, body);
    body->addCollisionDetectionPrimitive(new BoxCDP(pos1, pos2, body));

    body->setFrictionCoefficient(0.8);
    body->setRestitutionCoefficient(0.35);

    _world->addRigidBody(body);
}

void CartWheel3D::addBall(const string& name, const Vector3d& scale, double mass)
{
    string mesh = _path + "data/models/sphere10x.obj";
    Vector3d offset = Vector3d(0,0,0);

    RigidBody* body = new RigidBody();
    body->setName(name.c_str());
    body->setScale(scale);
    body->addMeshObj(mesh.c_str(), offset, scale);

    body->setColour(0.8,0,0,1);
    body->setMass(mass);
    body->setMOI(Vector3d(0.2,0.2,0.2));

    Point3d center = Point3d(0,0,0);
    SphereCDP* sphereCDP = new SphereCDP(center, 0.1, body);
    body->addCollisionDetectionPrimitive(sphereCDP);

    body->setFrictionCoefficient(1.8);
    body->setRestitutionCoefficient(0.35);

    _world->addRigidBody(body);
}

void CartWheel3D::loadBehaviourController(Character* ch, IKVMCController* c, double dHeading)
{
    TurnController* behaviour = new TurnController(ch, c, _oracle);
    behaviour->requestHeading(dHeading);
    behaviour->conTransitionPlan();

    c->setBehaviour(behaviour);
}

void CartWheel3D::runStep(double dt)
{
    for(unsigned int i=0; i<_controllers.size(); i++)
    {
        _controllers[i]->performPreTasks(dt, _world->getContactForces());
    }

    _world->advanceInTime(dt);

    for(unsigned int i=0; i<_controllers.size(); i++)
    {
        IKVMCController* c = _controllers[i];
        if( c->performPostTasks(dt, _world->getContactForces()) )
        {
            Vector3d step = Vector3d(c->getStanceFootPos(), c->getSwingFootPos());
            step = c->getCharacterFrame().inverseRotate(step);
            Vector3d v = c->getV();
//            double phi = c->getPhase();
            //printf("Human %d -> step: %f %f %f. Vel: %f %f %f phi = %f\n", i+1, step.x, step.y, step.z, v.x, v.y, v.z, phi);
        }
    }
}

Character* CartWheel3D::getAFtoCharacter(ArticulatedFigure* af)
{
    Character* ch = new Character();
    ch->setRoot(af->getRoot());

    for(int i=0; i<af->getJointCount(); i++)
    {
        ch->addJoint(af->getJoint(i));
        ch->addArticulatedRigidBody(af->getJoint(i)->getChild());
    }
    ch->computeMass();
    return ch;
}

