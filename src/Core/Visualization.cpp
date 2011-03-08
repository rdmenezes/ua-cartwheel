#include <sstream>
#include <Core/Visualization.h>
#include <Core/CartWheel3D.h>

namespace CartWheel {

Visualization* Visualization::g_instance = NULL;

using namespace Core;
using namespace Physics;
using GL::GLUtils;

using namespace std;

Visualization::Visualization(int width, int height) {
	g_instance = this;
    init(width, height);
}

Visualization::Visualization(void (*func)(void), int argc, char** argv, 
        int width, int height) {
	g_instance = this;
    init(width, height);
    initGlut(func, argc, argv);
}

Visualization::~Visualization() {
	g_instance = NULL;
	if (_menuIdentifier != -1){
		glutDestroyMenu(_menuIdentifier);
	}
	if (_humanMenu != -1){
		glutDestroyMenu(_humanMenu);
	}
}

void Visualization::init(int width, int height) {
    _humanSpeed = 0.0;
    _humanStepWidth = 0.0;
    _menuIdentifier = -1;
    _humanMenu = -1;
    _cw = NULL;
    _selectedHumanName = "";
    _width = width;
    _height = height;
    _renderGround = false;
    _captureImgs = false;
    _drawBB = false;

    _cameraPos = Point3d(0,0,0);
    _cameraTarget = Point3d(0,0,0);
    _cameraUp = Point3d(0,1,0);

    _mainWindow = NULL;
    _window = NULL;
}

static void timerProc(int) {
	glutPostRedisplay();
	glutTimerFunc(10, timerProc, 0);
}

static void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0) {
		h = 1;
	}

	float ratio = 1.0* w / h;

	Visualization::g_instance->setWidth(w);
	Visualization::g_instance->setHeight(h);
}

void Visualization::initGlut(void (*func)(void), int argc, char** argv) {    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(10,10);
    glutInitWindowSize(_width, _height);
    glutCreateWindow("Physics Visualization");
    glutDisplayFunc(func);
    glutIdleFunc(func);
	glutReshapeFunc(changeSize);
    glutTimerFunc(10, timerProc, 0);
}

static void processNormalKeys(unsigned char key, int x, int y) {
	x=x; y=y; // unused parameters

	if (key == 27)
		exit(0);

	if (NULL == Visualization::g_instance)
		return;

	CartWheel3D* cw = Visualization::g_instance->getCartWheelHandle();
	if (NULL == cw)
		return;

	string selectedHuman = Visualization::g_instance->getSelectedHuman();

	if (key >= 48 && key <= 57) { // numeric keys 0..9
		int actionIndex = key - 48;
		cw->setController(Visualization::g_instance->getSelectedHuman(), actionIndex);
	}
	else {
		switch(key)
		{
		case 65: // a
		case 97:
			cw->setHumanHeading(selectedHuman, -3.14/2);
			break;
		case 83: // s
		case 115:
			cw->setHumanHeading(selectedHuman, 3.14/2);
			break;
		case 87: // w
		case 119:
			cw->setHumanHeading(selectedHuman, 3.14);
			break;
		case 90: // z
		case 122:
			cw->setHumanHeading(selectedHuman, 0);
			break;
		case 82: // r
		case 114:
			cw->reset();
			break;
		case 45: //"-"
			Visualization::g_instance->setHumanSpeed(Visualization::g_instance->getHumanSpeed()-0.2);
			cw->setHumanSpeed(selectedHuman, Visualization::g_instance->getHumanSpeed());
			break;
		case 61: //"+"
			Visualization::g_instance->setHumanSpeed(Visualization::g_instance->getHumanSpeed()+0.2);
			cw->setHumanSpeed(selectedHuman, Visualization::g_instance->getHumanSpeed());
			break;
		case 44: //"<"
			Visualization::g_instance->setHumanStepWidth(Visualization::g_instance->getHumanStepWidth()-0.2);
			cw->setHumanStepWidth(selectedHuman, Visualization::g_instance->getHumanStepWidth());
			break;
		case 46: //">"
			Visualization::g_instance->setHumanStepWidth(Visualization::g_instance->getHumanStepWidth()+0.2);
			cw->setHumanStepWidth(selectedHuman, Visualization::g_instance->getHumanStepWidth());
			break;
		default:
			break;
		}
	}
}

void Visualization::initKeyboard() {
	glutKeyboardFunc(processNormalKeys);
}

#define RESET 1

#define SELECT_HUMAN 100
#define SELECT_CONTROLLER 200

static void processMenuEvents(int option) {

	if (NULL == Visualization::g_instance)
		return;

	if (option == RESET) {
		CartWheel3D* cw = Visualization::g_instance->getCartWheelHandle();
		if (NULL != cw) {
			cw->reset();
		}
	}
	else if (option >= SELECT_HUMAN && option < SELECT_CONTROLLER) {
		int indexHuman = option - SELECT_HUMAN;

		std::vector<std::string> humanNames;
		Visualization::g_instance->getHumanNames(humanNames);

		vector<string>::iterator itr = humanNames.begin();
		std::advance(itr, indexHuman);
		if (itr != humanNames.end()) {
			Visualization::g_instance->selectHuman((*itr));
		}
	}
}

void Visualization::initMenu() {

	// Need a handle to CartWheel for menu functionality
	if (NULL != _cw) {

		// create the submenu
		_humanMenu = glutCreateMenu(processMenuEvents);

		std::vector<std::string> humanNames;
		bool result = _cw->getHumanNames(humanNames);

		Visualization::g_instance->setHumanNames(humanNames);

		vector<string>::iterator itr = humanNames.begin();
		// Select the first human by default
		_selectedHumanName = (*itr);

		for (int id = SELECT_HUMAN; itr != humanNames.end(); itr++,id++)
		{
			glutAddMenuEntry((*itr).c_str(), id);
		}

		// create the menu
		_menuIdentifier = glutCreateMenu(processMenuEvents);

		//add entries to our menu
		glutAddMenuEntry("Reset", RESET);
		glutAddSubMenu("Select", _humanMenu);

		// attach the menu to the right button
		glutAttachMenu(GLUT_RIGHT_BUTTON);
	}
}

void Visualization::initGL(const Point3d& cameraPos, const Point3d& cameraTarget) {
    setCameraLocation(cameraPos);
    setCameraTarget(cameraTarget);

    _window = new GLUITopLevelWindow(_width,_height);
    _mainWindow = new GLUIWindow(_window, 0, 0, _width, _height);

    initGLParams();
}

void Visualization::initGLParams() { 
	glClearColor(0.0, 0.0, 0.0, 1);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	glBlendFunc(GL_SRC_ALPHA ,GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.0, 0.0, 0.0, 1);

	float ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f};
	float diffuse0[] = { 0.9, 0.9, 0.9, 1.0 };
	float diffuse[] = { 0.6, 0.6, 0.6, 1.0 };
	float specular[] = { 0.0, 0.0, 0.0, 0.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular);

	glLightfv(GL_LIGHT2, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT2, GL_SPECULAR, specular);

	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50.0);

	glEnable(GL_LIGHTING);

	float light0_position[] = { 50.0, 10.0, 200.0, 0.0 };
	float light1_position[] = {200.0, 10.0, -200.0, 0.0 };
	float light2_position[] = {-200.0, 10.0, -200.0, 0.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glLightfv(GL_LIGHT2, GL_POSITION, light2_position);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
}

void Visualization::applyCameraTransforms() { 
    gluLookAt(_cameraPos.x, _cameraPos.y, _cameraPos.z,
            _cameraTarget.x, _cameraTarget.y, _cameraTarget.z, 
            _cameraUp.x, _cameraUp.y, _cameraUp.z);

    // Update the World to Camera transformation matrix
    double vals[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, vals);
    _worldToCam.setOGLValues(vals);
}

void Visualization::captureImage() {
    static int counter = 0;
//    FILE* fptr;
    printf("Capturing Frames %d...\n", counter);

    counter++;
    char fname[32];
    sprintf(fname, "C_%04d.jpg", counter);
    //GLUtils::saveScreenShot(fname, 0, 0, _width, _height);
}

void Visualization::render(CartWheel3D* cartwheel) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, _width/float(_height), 0.1,150.0);
    applyCameraTransforms();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (_renderGround) {
        // TODO: ensure that sPath is a global variable somewhere
        // TODO: determine if we should be loading the bmp file every
        //  time that we want to draw the ground.
        
    	GLUtils::drawGround(50, 5, 98);

    	//GLUtils::gprintf( "_humanSpeed %d", _humanSpeed);
    }

    glEnable(GL_LIGHTING);

    World* world = cartwheel->getWorld();
    world->drawRBs(SHOW_COLOURS | SHOW_MESH);

    if (_captureImgs) { 
        captureImage();
    }

    glDisable(GL_LIGHTING);
    glFlush();

    glutSwapBuffers();
}

void Visualization::glutStep() {
    glutMainLoopEvent();
}

void Visualization::drawAxes(void) {
	glPushMatrix();
	glLoadIdentity();
	glTranslated(-3,-2.0,-6.0);
    // TODO: determine what this is really doing?
	//glRotated(-180/M_PI * rotations.x,1.0,0.0,0.0);
	//glRotated(-180/M_PI * rotations.y,0.0,1.0,0.0);
	//glRotated(-180/M_PI * rotations.z,0.0,0.0,1.0);
	glDisable(GL_DEPTH_TEST);
	GLUtils::drawAxes(0.5);
	glEnable(GL_DEPTH_TEST);
	glPopMatrix();
}

// TODO: resurrect bounding box
#if 0 
void Visualization::fillHumanVertices(CartWheel3D* cartwheel, int i, vector<Point3d>& vertices) { 
    Character* character = cartwheel->getHuman(i)->getCharacter();
    vector<string> parts;
    parts.push_back("torso");
    parts.push_back("pelvis");
    parts.push_back("lowerBack");
    parts.push_back("head");
    parts.push_back("lUpperLeg");
    parts.push_back("lLowerLeg");
    parts.push_back("rUpperLeg");
    parts.push_back("rLowerLeg");
   
    for (size_t i = 0; i < parts.size(); ++i) {
        BoundingBox bb = getLimbBox(character, parts[i]);
        for (int j = 0; j < 8; ++j) { 
            vertices.push_back(bb.getVertex(j));
        }
    }
}

BoundingBox Visualization::getLimbBox(CartWheel::Core::Character* human, string name) {
    ArticulatedRigidBody* arb = human->getARBByName(name.c_str());
    assert(arb != NULL);

    Point3d position = arb->getCMPosition();
    CapsuleCDP* cdp = (CapsuleCDP*) arb->getCDP(0);

    Point3d p1 = arb->getWorldCoordinates(cdp->getPoint1());
    Point3d p2 = arb->getWorldCoordinates(cdp->getPoint2());

    return BoundingBox(p1, p2, cdp->getRadius());
}

void Visualization::projectVertices(kjb::Bounding_Box2D& bb, vector<Point3d> vertices) { 
    double fov = 45.0 * (M_PI / 180.0); // 45 degrees in radians
    double focal_length = (_height / 2.0) / tan(fov / 2.0);

    kjb::Perspective_camera camera(0.1, 150);
    camera.set_focal_length(focal_length);
    camera.set_look_at(_cameraPos.x, _cameraPos.y, _cameraPos.z, 
            _cameraTarget.x, _cameraTarget.y, _cameraTarget.z, 
            _cameraUp.x, _cameraUp.y, _cameraUp.z);
    camera.prepare_for_rendering(true);

    // convert the CartWheel points into kjb points
    vector<kjb::Vector> points3D;
    for (size_t i = 0; i < vertices.size(); ++i) { 
        points3D.push_back(kjb::Vector(4,1.0));
        points3D[i](0) = vertices[i].x;
        points3D[i](1) = vertices[i].y;
        points3D[i](2) = vertices[i].z;
    }
    
    try { 
        kjb::get_projected_bbox_from_3Dpoints(bb, points3D,
                camera.get_rendering_interface(), _width, _height);
    } catch (kjb::KJB_error e) { 
        throw(e);
    }

}
#endif

void Visualization::mainLoop() {
	glutMainLoop();
}

// TODO: render the projected box overlayed onto the cartwheel image 

}
