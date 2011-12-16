
#include <Core/CartWheel3D.h>
#include <GLUtils/GLUtils.h>
#include <Core/TurnController.h>
#include <Core/ActionCollectionPolicy.h>
#include <Core/CompositeBehaviourController.h>

#include <iostream>
#include <sstream>

using namespace std;
using std::vector;

using namespace CartWheel;
using namespace CartWheel::Core;
using namespace CartWheel::GL;
using namespace CartWheel::Physics;
using namespace CartWheel::Math;
using namespace CartWheel::Util;
using namespace CartWheel::Behaviors;

CartWheel3D::CartWheel3D() :
_path(""), _world(&World::instance()), _oracle(NULL),
_builderFunction(NULL), _nTime(0) {
    _oracle = new WorldOracle();
    _oracle->initializeWorld(_world);
    _behaviorsManager = new BehaviorsManager();
}

CartWheel3D::CartWheel3D(const string& dataPath) :
_path(dataPath), _world(&World::instance()), _oracle(NULL),
_builderFunction(NULL), _nTime(0) {
    _oracle = new WorldOracle();
    _oracle->initializeWorld(_world);
    _behaviorsManager = new BehaviorsManager();
}

CartWheel3D::~CartWheel3D() {
    _humans.clear();
    _boxes.clear();
    _hStates.clear();

    delete _oracle;
    _world->destroyAllObjects();
    delete _behaviorsManager;
}

void CartWheel3D::addHuman(const string& name, const string& characterFile, const string& controllerFile, const Point3d& pos, double heading) {
    bool foundMatch = (_humans.find(name) != _humans.end());
    if (foundMatch) {
        ostringstream osError;
        osError << name << " already exists!";
        throwError(osError.str().c_str());
    }

    string sFile = _path + characterFile;
    _world->loadRBsFromFile(sFile.c_str(), _path.c_str(), name.c_str());
    Character* ch = new Character(_world->getAF(_world->getAFCount() - 1));

    IKVMCController* c = new IKVMCController(ch);

    sFile = _path + controllerFile;
    c->loadFromFile(sFile.c_str());

    BehaviourController* behaviour = new TurnController(ch, c, _oracle);

    Human* human = new Human(name, ch, c, behaviour);
    // Initialize
    human->init();

    human->setHeading(heading);
    human->setPosition(pos);

    ch->setHeading(heading);

    _humans[name] = human;
    ch->getState(&_hStates[name]);
    setController(name, 0);
//    doBehavior("Standing", name, NULL);
}

void CartWheel3D::addHuman(const string& name, const std::string& characterFile, const std::string& controllerFile, const std::string& actionFile,
        const Math::Point3d& pos, double heading) {
    bool foundMatch = (_humans.find(name) != _humans.end());
    if (foundMatch) {
        ostringstream osError;
        osError << name << " already exists!";
        throwError(osError.str().c_str());
    }

    string sFile = _path + characterFile;
    _world->loadRBsFromFile(sFile.c_str(), _path.c_str(), name.c_str());
    Character* ch = new Character(_world->getAF(_world->getAFCount() - 1));
//    printf("AF: %s\n", _world->getAF(_world->getAFCount() - 1)->getName());
    ArticulatedRigidBody* body = ch->getArticulatedRigidBody(4);
    printf("Trying to get a Body\n");
    if (body==NULL)
        printf("Body not found\n");
    else {
        printf("Body found!!!\n");
////        body->addMeshObj("data/models/bipv3/head.obj", Vector3d(0,0,0), Vector3d(0.5,2,0.5));
//        body->replaceMeshObj("data/models/box3.obj");
//        body->getMesh(0)->scale(Vector3d(0.06,0.06,0.06));
//        body->getMesh(0)->computeNormals();
//        body->setColour(1,0,0,1);
//        body->setMass(-10);
    }

    CompositeController* con = new CompositeController(ch, _oracle, controllerFile.c_str());
    ActionCollectionPolicy* policy = new ActionCollectionPolicy(con);
    policy->loadActionsFromFile(actionFile.c_str());

    // Create a new human
    Human* human = new Human(name, ch, con, policy);
    // Initialize
    human->init();

    human->setHeading(heading);
    human->setPosition(pos);

    ch->setHeading(heading);

    _humans[name] = human;
    ch->getState(&_hStates[name]);
//    doBehavior("Standing", name, NULL);
    setController(name, 0);
    
    
//    printf("\n\nJoints:\n");
//    for(int i=0; i<ch->getJointCount(); i++) {
//        Point3d p1 = ch->getJoint(i)->getChildJointPosition();
//        Point3d p2 = ch->getJoint(i)->getParentJointPosition();
//        printf("Joint: %s, Parent-Joint: %s, Pos: (%f, %f, %f)\n", ch->getJoint(i)->getName(), ch->getJoint(i)->getParent()->getName(), p2.x, p2.y, p2.z);
//        printf("Joint: %s, Child-Joint: %s, Pos: (%f, %f, %f)\n\n", ch->getJoint(i)->getName(), ch->getJoint(i)->getChild()->getName(), p1.x, p1.y, p1.z);
//    }
    printf("\n\n\nArticulated Rigid Bodies:\n");
    for(int i=0; i<ch->getArticulatedRigidBodyCount(); i++) {
        Vector3d p = ch->getArticulatedRigidBody(i)->getCMPosition();
        printf("ARB: %s, Pos: (%f, %f, %f)\n", ch->getArticulatedRigidBody(i)->getName(), p.x, p.y, p.z);
    }
}

void CartWheel3D::resetHumanPose(const std::string& name) {
    makeHumanDropObject(name, "ball1");

    ReducedCharacterStateArray state, cState;
    state = _hStates[name];
    _humans[name]->getCharacter()->getState(&cState);

    ReducedCharacterState chS(&state);
    ReducedCharacterState current_chS(&cState);
    Math::Vector3d pos = chS.getPosition();
    pos.x = current_chS.getPosition().x;
    pos.z = current_chS.getPosition().z;
    chS.setPosition(pos);

    double current_Heading = _humans[name]->getCharacter()->getHeadingAngle();
    Quaternion qHuman;
    qHuman = chS.getOrientation();
    qHuman.setToRotationQuaternion(current_Heading, Vector3d(0, 1, 0));
    chS.setOrientation(qHuman);

    _humans[name]->getCharacter()->setState(&state);
}

void CartWheel3D::setHumanPosition(const std::string& name, const Math::Vector3d& pos, double heading = NULL) {
    makeHumanDropObject(name, "ball1");

    ReducedCharacterStateArray state;
    state = _hStates[name];

    Quaternion qHuman;
    ReducedCharacterState chS(&state);
    chS.setPosition(pos);

    if (heading != NULL) {
        qHuman = chS.getOrientation();
        qHuman.setToRotationQuaternion(heading, Vector3d(0, 1, 0));
        chS.setOrientation(qHuman);
        _humans[name]->getCharacter()->setHeading(heading);
        _humans[name]->setHeading(heading);
    }
    _humans[name]->getCharacter()->setState(&state);
}

void CartWheel3D::addObject(const string& name, const string& objFile, double mass) {
    string sFile = _path + objFile;
    _world->loadRBsFromFile(sFile.c_str(), _path.c_str(), name.c_str());

    RigidBody* rb = _world->getRB(_world->getRBCount() - 1);
//    printf("renaming %s into %s\n", rb->getName(), name.c_str());
//    RigidBody* rb2;
//    if (_world->getRBCount()>1)
//        rb2 = _world->getRB(_world->getRBCount() - 2);
//        if (rb2!=NULL)
//            printf("rb %s\n", rb2->getName());
    rb->setName(name.c_str());

    if (mass > 0) {
        rb->setMass(mass);
    }
}

void CartWheel3D::addBox(const string& name, const Vector3d& scale, const Vector3d& position, 
        double rotation, double mass, double friction, double restitution) {
    string mesh = _path + "data/models/box3.obj";
    Vector3d offset = Vector3d(0, 0, 0);

    ArticulatedRigidBody* body = new ArticulatedRigidBody();
    body->setName(name.c_str());
    body->setScale(scale);
    body->addMeshObj(mesh.c_str(), offset, scale);
    body->setColour(0.8, 0.8, 0.8, 1);
    body->setMass(mass);
    body->setMOI(Vector3d(0.2, 0.2, 0.2));

    Point3d pos1 = Point3d(-scale.x, -scale.y, -scale.z);
    Point3d pos2 = Point3d(scale.x, scale.y, scale.z);

    body->addCollisionDetectionPrimitive(new BoxCDP(pos1, pos2, body));

    body->setFrictionCoefficient(friction);
    body->setRestitutionCoefficient(restitution);

    body->setCMPosition(position);
    Vector3d y_axis(0, 1, 0);
    body->setOrientation(rotation, y_axis);

    //  ArticulatedRigidBody* arb = dynamic_cast<ArticulatedRigidBody*> (body);
    //  if (NULL != arb)
    //  {
    body->setParentJoint(NULL);
    _world->addArticulatedRigidBody(body);

    _boxes[name] = body;
    //  }
    //  else
    //  {
    //    _world->addRigidBody(body);
    //  }
}

void CartWheel3D::addBox(const string& name, const Vector3d& scale, double mass, 
        double friction, double restitution) {
    Vector3d position = Vector3d(0, 0, 0);
    addBox(name, scale, position, 0.0, mass, friction, restitution);
}

void CartWheel3D::addBox(const string& name, const Vector3d& scale, double mass) {
    Vector3d position = Vector3d(0, 0, 0);
    addBox(name, scale, position, 0.0, mass);
}

void CartWheel3D::addBall(const string& name, const Vector3d& scale, double mass, const Vector3d& color) {
    string mesh = _path + "data/models/sphere10x.obj";
    Vector3d offset = Vector3d(0, 0, 0);
#if 1
    RigidBody* body = new ArticulatedRigidBody();
#else
    RigidBody* body = new RigidBody();
#endif

    body->setName(name.c_str());
    body->setScale(scale);
    body->addMeshObj(mesh.c_str(), offset, scale);
    body->setColour(color.x, color.y, color.z, 1);
//    body->setColour(0.1, 0, 0.8, 1);
    body->setMass(mass);
    body->setMOI(Vector3d(0.2, 0.2, 0.2));

    Point3d center = Point3d(0, 0, 0);
    SphereCDP* sphereCDP = new SphereCDP(center, scale.getX(), body);
    body->addCollisionDetectionPrimitive(sphereCDP);

    body->setFrictionCoefficient(1.8);
    body->setRestitutionCoefficient(0.35);

    ArticulatedRigidBody* arb = dynamic_cast<ArticulatedRigidBody*> (body);
    if (NULL != arb) {
        arb->setAFParent(NULL);
        arb->setParentJoint(NULL);
        _world->addArticulatedRigidBody(arb);
    } else {
        _world->addRigidBody(body);
    }
}

void CartWheel3D::addBehaviorObject(const std::string& name, const std::string& objFile, 
        const Math::Vector3d& scale, double mass, double friction, double restitution) {
    if(!objFile.empty()) {
        addObject(name, objFile, mass);
    } else {
        addBall(name, scale, mass);
    }
    if(friction >= 0)
        getObjectByName(name)->setFrictionCoefficient(friction);
    if(restitution >= 0)
        getObjectByName(name)->setRestitutionCoefficient(restitution);
    
    _objects.push_back(name);
}

void CartWheel3D::updateRB(const string& name, const Point3d& pos, const Quaternion& orientation, const Vector3d& vel) {
    RigidBody* body = _world->getRBByName(name.c_str());
    body->setCMPosition(pos);
    body->setOrientation(orientation);
    body->setCMVelocity(vel);
}

void CartWheel3D::reset() {
    _humans.clear();

    _world->destroyAllObjects();

    if (NULL != _builderFunction) {
        _builderFunction(this);
    }
    _nTime = 0;
    _behaviorsManager->reset();

    //    if(_oracle != NULL)
    //        delete _oracle;
    //
    //    _oracle = new WorldOracle();
    //    _oracle->initializeWorld(_world);
}

double CartWheel3D::getTotalSerialTime() {
    return _behaviorsManager->getTotalSerialTime();
}

void CartWheel3D::doSerialBehavior(string behaviorName, string humanName, Behaviors::Params* params) {
    _behaviorsManager->createSerialBehavior(behaviorName, humanName, params, this);
}

void CartWheel3D::doParallelBehavior(string behaviorName, string humanName, Behaviors::Params* params) {
    _behaviorsManager->createParallelBehavior(behaviorName, humanName, params, this);
} 

void CartWheel3D::doBehavior(string behaviorName, string humanName, Behaviors::Params* params) {
//    if(strcmp(behaviorName.c_str(), "MoveHand_IK")==0) {
//        doSerialBehavior("PickUp", humanName, &Behaviors::PickUp_Params(0, 50, "ball1", "Left"));
//    } else if(strcmp(behaviorName.c_str(), "Kick")==0) {
//        doSerialBehavior("Kick", humanName, &Behaviors::Kick_Params(1.5, 1.2));
//    } else if(strcmp(behaviorName.c_str(), "Push")==0) {
//        doSerialBehavior("Push", humanName, &Behaviors::Push_Params(1, 2));
//    } else if(strcmp(behaviorName.c_str(), "WaveHand")==0) {
//        doSerialBehavior("WaveHand", humanName, &Behaviors::WaveHand_Params(1, 2, "Left"));
//    } else if(strcmp(behaviorName.c_str(), "Dig")==0) {
//        doSerialBehavior("Dig", humanName, &Behaviors::Dig_Params(1.5, 7));
//    }
    _behaviorsManager->createBehavior(behaviorName, humanName, params, this);
}

bool CartWheel3D::isBehaviorsDone() {
    return _behaviorsManager->isBehaviorsDone();
}

void CartWheel3D::runStep(double dt) {
    
    _nTime += dt;
    DynamicArray<ContactPoint>* contactPoints = _world->getContactForces();
    DynamicArray<Vector3d> humanPositions;
    
    for (HumanItr itr = _humans.begin(); itr != _humans.end(); itr++) {
        Human* human = (*itr).second;
        SimBiController* c = human->getController();
        if (NULL != c) {
            c->performPreTasks(dt, contactPoints);

            // Save the current position
            humanPositions.push_back(human->getPosition());

            BehaviourController* b = human->getBehaviour();
            if (NULL != b) {
                if (b->shouldAbort()) {

                }
            }
        }
    }
    _world->advanceInTime(dt);

    bool isHumansWorking = false;
    bool isObjsWorking = false;
    contactPoints = _world->getContactForces();
    for (HumanItr itr = _humans.begin(); itr != _humans.end(); itr++) {
        Human* human = (*itr).second;
        SimBiController* c = human->getController();

        if (NULL != c) {
            c->performPostTasks(dt, contactPoints);

            // Save the current position
            humanPositions.push_back(human->getPosition());

            BehaviourController* b = human->getBehaviour();
            if (NULL != b) {
                if (b->shouldAbort()) {

                }
            }
        }        
        if(_behaviorsManager->runStep(human->getName().c_str(), _nTime)) {
//            printf("nTime: %f\n", _nTime);
            isHumansWorking = true;
        }
    }
    for(int i=0; i<_objects.size(); i++) {
        if(_behaviorsManager->runStep(_objects[i].c_str(), _nTime)) {
            isObjsWorking = true;
        }
    }
    if(!isHumansWorking && !isObjsWorking) {
        _behaviorsManager->setBehaviorsDone(true);
    } 
}

Math::Vector3d CartWheel3D::getHumanPosition(const std::string& name) {
    return _humans[name]->getPosition();
}

double CartWheel3D::getHumanHeading(const std::string& name) {
    return _humans[name]->getHeading();
}

Math::Vector3d CartWheel3D::getHumanVelocity(const std::string& name) {
    return _humans[name]->getVelocity();
}

bool CartWheel3D::getHuman(const std::string& name, Human** human) {
    bool foundMatch = false;

    foundMatch = (_humans.find(name) != _humans.end());

    if (foundMatch) {
        *human = _humans[name];
    }

    return foundMatch;
}

int CartWheel3D::getController(const std::string& name) {
    int actionIndex = 0;
    Human* human = _humans[name];

    ActionCollectionPolicy* policy = dynamic_cast<ActionCollectionPolicy*> (human->getPolicy());
    if (NULL != policy) {
        actionIndex = policy->getActionIndex();
    }

    return actionIndex;
}

bool CartWheel3D::getHumanNames(std::vector<std::string>& names) {
    for (HumanItr itr = _humans.begin(); itr != _humans.end(); itr++) {
        names.push_back((*itr).first);
    }

    bool result = names.size() > 0;

    return result;
}

bool CartWheel3D::getBoxNames(std::vector<std::string>& name) {
    for (std::map<std::string, Physics::ArticulatedRigidBody*>::iterator bx = _boxes.begin(); bx != _boxes.end(); ++bx) {
        name.push_back(bx->first);
    }
}

bool CartWheel3D::getHumanNames(std::list<std::string>& names) {
    for (HumanItr itr = _humans.begin(); itr != _humans.end(); itr++) {
        names.push_back((*itr).first);
    }

    bool result = names.size() > 0;

    return result;
}

int CartWheel3D::getHumanCount() {
    return _humans.size();
}

const std::string& CartWheel3D::getPath() {
    return _path;
}

// TODO: This will probably give an error because it's not const.

Physics::World* CartWheel3D::getWorld() {
    return _world;
}

Physics::RigidBody* CartWheel3D::getObjectByName(const std::string& name) {
    if (_world->getRBByName(name.c_str())==NULL)
        printf("Not object [CartWheel3D::getObjectByName] found by name %s.\n", name.c_str());
    return _world->getRBByName(name.c_str());
}

Physics::ArticulatedRigidBody* CartWheel3D::getARBObjectByName(const std::string& nameARB, const std::string& nameAF) {
    if (_world->getARBByName(nameARB.c_str(), nameAF.c_str())==NULL)
        printf("Not ARBobject [CartWheel3D::getARBObjectByName] found by name %s [from AF: %s].\n", 
                nameARB.c_str(), nameAF.c_str());
    return _world->getARBByName(nameARB.c_str(), nameAF.c_str());
}

void CartWheel3D::setHumanPosition(const std::string& name, const Math::Point3d& pos) {
    _humans[name]->setPosition(pos);

    //_world->getAF(i)->getRoot()->setCMPosition(pos);
}

void CartWheel3D::setHumanHeading(const std::string& name, double angle) {
    _humans[name]->setHeading(angle);
}

void CartWheel3D::setHumanSpeed(const std::string& name, double speed) {
    _humans[name]->setSpeed(speed);
}

void CartWheel3D::setHumanStepWidth(const std::string& name, double width) {
    _humans[name]->setStepWidth(width);
}

void CartWheel3D::setController(const std::string& name, int actionIndex) {
    _humans[name]->applyAction(actionIndex);
}

/*
Human::GrabbingMethod method = Human::right;
_humans[name]->grabObject("ball1", Human::right);
 */

void CartWheel3D::makeHumanGrabObject(const std::string& name, const std::string& targetName, const Human::GrabbingMethod& method) {
    _humans[name]->grabObject(targetName, method);
}

void CartWheel3D::makeHumanThrowObject(const std::string& name, const std::string& targetName, const Math::Vector3d& velocity) {
    _humans[name]->throwObject(targetName, velocity);
}

void CartWheel3D::makeHumanDropObject(const std::string& name, const std::string& targetName) {
    _humans[name]->dropObject(targetName);
}
