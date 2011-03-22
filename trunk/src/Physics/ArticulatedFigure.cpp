#include <Physics/ArticulatedFigure.h>
#include <Physics/World.h>
#include <Physics/RBUtils.h>
#include <Utils/Utils.h>

using namespace CartWheel;
using namespace CartWheel::Physics;
using namespace CartWheel::Util;

using namespace std;

/**
	Default constructor
*/
ArticulatedFigure::ArticulatedFigure(void){
	root = NULL;
	name[0] = '\0';
	mass = 0;
}

char ArticulatedFigure::prefixDelimiter = ' ';

ArticulatedFigure::~ArticulatedFigure(void){
	//delete all the joints
	if (root != NULL)
		delete root;
	for (unsigned int i=0;i<arbs.size();i++)
		delete arbs[i];
	for (unsigned int i=0;i<joints.size();i++)
		delete joints[i];
	arbs.clear();
	joints.clear();
}

void ArticulatedFigure::loadIntoWorld() {
	World& world = World::instance();

	if( root == NULL )
		throwError( "Articulated figure needs a root before it can be loaded into the world!" );
	world.addRigidBody(root);
	for (unsigned int i=0;i<arbs.size();i++)
		world.addRigidBody(arbs[i]);
}

/**
	Sets the root
*/
void ArticulatedFigure::setRoot( ArticulatedRigidBody* root ) {
	if (this->root != NULL)
		throwError("This articulated figure already has a root");
    root->setAFParent( this );
	this->root = root;
}

void ArticulatedFigure::addArticulatedRigidBody( ArticulatedRigidBody* arb ) {
    //printf("ARB Name: %s\n", arb->getName()); 
	//printf("Before arbs: size=%d, capacity=%d\	n", arbs.size(), arbs.capacity());
    arb->setAFParent( this );
	arbs.push_back( arb );
	//printf("After arbs: size=%d, capacity=%d\n\n", arbs.size(), arbs.capacity());
}

bool ArticulatedFigure::removeArticulatedRigidBody( ArticulatedRigidBody* arb ) {
	bool result = false;
	if (NULL != arb) {
		arb->setAFParent(NULL);
		DynamicArray<ArticulatedRigidBody*>::iterator itr = arbs.begin();
		for(; itr != arbs.end(); itr++) {
			if (strcmp((*itr)->getName(), arb->getName()) == 0) {
				arbs.erase(itr);
				result = true;
				break;
			}
		}
	}
	return result;
}

void ArticulatedFigure::addJoint( Joint* jt ) {
	joints.push_back( jt );
}

bool ArticulatedFigure::removeJoint( Joint* jt ) {
	bool result = false;
	if (NULL != jt) {
		DynamicArray<Joint*>::iterator itr = joints.begin();
		for(; itr != joints.end(); itr++) {
			if (strcmp((*itr)->getName(), jt->getName()) == 0) {
				joints.erase(itr);
				result = true;
				break;
			}
		}
	}
	return result;
}

/**
	This method is used to automatically fix the errors in the joints (i.e. drift errors caused by numercial integration). At some future
	point it can be changed into a proper stabilization technique.
*/
void ArticulatedFigure::fixJointConstraints(bool fixOrientations, bool fixVelocities){
	if (!root)
		return;

	for (unsigned int i=0;i<root->cJoints.size();i++)
		root->cJoints[i]->fixJointConstraints(fixOrientations, fixVelocities, true);
}


/**
	This method is used to compute the total mass of the articulated figure.
*/
void ArticulatedFigure::computeMass(){
	double curMass = root->getMass();
	double totalMass = curMass;

	for (unsigned int i=0; i < joints.size(); i++){
		curMass = joints[i]->child->getMass();
		totalMass += curMass;
	}

	mass = totalMass;
}

/**
	This method is used to get the total mass of the articulated figure.
*/
double ArticulatedFigure::getMass(){
	return mass;
}

/**
	This method is used to get all the joints in the articulated figure and add them to the list of joints that is passed in as a paramter.
*/
void ArticulatedFigure::addJointsToList(DynamicArray<Joint*> *joints){
	if (!root)
		return;
	DynamicArray<ArticulatedRigidBody*> bodies;
	bodies.push_back(root);

	int currentBody = 0;

	while ((unsigned int)currentBody < bodies.size()){
		//add all the children joints to the list
		for (unsigned int i=0; i < bodies[currentBody]->cJoints.size(); i++){
			joints->push_back(bodies[currentBody]->cJoints[i]);
			bodies.push_back(bodies[currentBody]->cJoints[i]->child);
		}
		currentBody++;
	}
}

/**
	This method is used to load the details of an articulated figure from file. The PhysicalWorld parameter points to the world in which the objects
	that need to be linked live in.
*/
void ArticulatedFigure::loadFromFile(FILE* f, World* world){
	if (f == NULL)
		throwError("Invalid file pointer.");
	if (world == NULL)
		throwError("A valid physical world must be passed in as a parameter");
	//have a temporary buffer used to read the file line by line...
	char buffer[200];
	char tempName[100];
	Joint* tempJoint;

	//this is where it happens.
	while (!feof(f)){
		//get a line from the file...
		fgets(buffer, 200, f);
		if (strlen(buffer)>195)
			throwError("The input file contains a line that is longer than ~200 characters - not allowed");
		char *line = lTrim(buffer);
		int lineType = getRBLineType(line);
		switch (lineType) {
			case RB_ROOT:
				sscanf(line, "%s", tempName);
				if (root != NULL)
					throwError("This articulated figure already has a root");
				root = world->getARBByName(tempName);
				if (root == NULL)
					throwError("The articulated rigid body \'%s\' cannot be found!", tempName);
				break;
			case RB_JOINT_TYPE_UNIVERSAL:
				tempJoint = new UniversalJoint();
				tempJoint->readAxes( line );
				tempJoint->loadFromFile(f, world);
				tempJoint->child->AFParent = this;
				tempJoint->parent->AFParent = this;
				//printf("Reading Joint: %s\n", tempJoint->getName());
				joints.push_back(tempJoint);
				arbs.push_back(tempJoint->getChild());
				break;
			case RB_JOINT_TYPE_HINGE:
				tempJoint = new HingeJoint();
				tempJoint->readAxes( line );
				tempJoint->loadFromFile(f, world);
				tempJoint->child->AFParent = this;
				tempJoint->parent->AFParent = this;
				//printf("Reading Joint: %s\n", tempJoint->getName());
				joints.push_back(tempJoint);
				arbs.push_back(tempJoint->getChild());
				break;
			case RB_JOINT_TYPE_BALL_IN_SOCKET:
				tempJoint = new BallInSocketJoint();
				tempJoint->readAxes( line );
				tempJoint->loadFromFile(f, world);
				tempJoint->child->AFParent = this;
				tempJoint->parent->AFParent = this;
				//printf("Reading Joint: %s\n", tempJoint->getName());
				joints.push_back(tempJoint);
				arbs.push_back(tempJoint->getChild());
				break;
			case RB_END_ARTICULATED_FIGURE:
				//make sure that the root does not have a parent, otherwise we'll end up with loops in the articulated figure]
				if (root->pJoint != NULL)
					throwError("The root of the articulated figure is not allowed to have a parent!");
				return;//and... done
				break;
			case RB_NOT_IMPORTANT:
				if (strlen(line)!=0 && line[0] != '#')
					printf("Ignoring input line: \'%s\'\n", line);
				break;
			default:
				throwError("Incorrect articulated body input file: \'%s\' - unexpected line.", buffer);
		}
	}
	throwError("Incorrect articulated body input file! No /ArticulatedFigure found");
}

void ArticulatedFigure::prefixARBNames(const char* prefixName) {

	if (root != NULL) {
		std::ostringstream ostrRoot;
		ostrRoot << prefixName << prefixDelimiter << root->name;
		strcpy(root->name, ostrRoot.str().c_str());
	}

	for (unsigned int i=0;i<arbs.size();i++) {
		std::ostringstream ostr;
		ostr << prefixName << prefixDelimiter << arbs[i]->name;
		strcpy(arbs[i]->name, ostr.str().c_str());
	}
}

std::string ArticulatedFigure::getNoPrefixName(const std::string& name) {

	vector<string> elems;
	stringstream ss(name);
	string item;

	while(getline(ss, item, prefixDelimiter)) {
		elems.push_back(item);
	}

	string newName = elems[elems.size()-1];
	return newName;
}


/**
	This method returns an ARB that is a child of this articulated figure
*/
ArticulatedRigidBody* ArticulatedFigure::getARBByName(const char* name) const {
	std::string findName(name);

	if( root != NULL ) {
		std::string rootName = getNoPrefixName(root->name);
		if (findName == rootName)
			return root;
	}

	for (unsigned int i=0;i<arbs.size();i++) {
		std::string arbsName = getNoPrefixName(arbs[i]->name);
		if (findName == arbsName) {
			return arbs[i];
		}
	}
	return NULL;
}
