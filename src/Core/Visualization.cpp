#include <sstream>
#include <Core/Visualization.h>
#include <Core/CartWheel3D.h>

#include "Behaviors/WalkInArc.h"

namespace CartWheel {

    Visualization* Visualization::g_instance = NULL;

    using namespace Core;
    using namespace Math;
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
        if (_menuIdentifier != -1) {
            glutDestroyMenu(_menuIdentifier);
        }
        if (_humanMenu != -1) {
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

        _cameraPos = Point3d(0, 0, 0);
        _cameraTarget = Point3d(0, 0, 0);
        _cameraUp = Point3d(0, 1, 0);
        _cameraFovy = 45.0;

        _mainWindow = NULL;
        _window = NULL;

        _postRenderCallback = NULL;

        srand(time(NULL));
    }

    static void timerProc(int) {
        glutPostRedisplay();
        glutTimerFunc(10, timerProc, 0);
    }

    static void changeSize(int w, int h) {

        // Prevent a divide by zero, when window is too short
        // (you cant make a window of zero width).
        if (h == 0) {
            h = 1;
        }

        float ratio = 1.0 * w / h;

        Visualization::g_instance->setWidth(w);
        Visualization::g_instance->setHeight(h);
    }

    void Visualization::initGlut(void (*func)(void), int argc, char** argv) {
        glutInit(&argc, argv);
        glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
        glutInitWindowPosition(10, 10);
        glutInitWindowSize(_width, _height);
        glutCreateWindow("Physics Visualization");
        glutDisplayFunc(func);
        glutIdleFunc(func);
        glutReshapeFunc(changeSize);
        glutTimerFunc(10, timerProc, 0);
    }

    static void processNormalKeys(unsigned char key, int x, int y) {
        x = x;
        y = y; // unused parameters

        if (key == 27)
            exit(0);

        if (NULL == Visualization::g_instance)
            return;

        CartWheel3D* cw = Visualization::g_instance->getCartWheelHandle();
        if (NULL == cw)
            return;

        string selectedHuman = Visualization::g_instance->getSelectedHuman();
        Human* human = NULL;

        if (key >= 48 && key <= 57) { // numeric keys 0..9
            int actionIndex = key - 48;
            //            cw->setController(Visualization::g_instance->getSelectedHuman(), actionIndex);

            Visualization::g_instance->_isNewBehavior = true;

            switch (actionIndex) {
                case 0:
                    Visualization::g_instance->_behaviorName = "Standing";
//                    Visualization::g_instance->_behaviorParams = new Standing_Params(0, 0);
                    break;
                case 1:
                    Visualization::g_instance->_behaviorName = "Kick";
//                    Visualization::g_instance->_behaviorParams = new Kick_Params(0, 0);
                    break;
                case 2:
                    Visualization::g_instance->_behaviorName = "WalkInArc";
//                    Visualization::g_instance->_behaviorParams = new WalkInArc_Params(1, 50, 0.5, 2);
                    break;
                case 3:
                    Visualization::g_instance->_behaviorName = "Walk";
//                    Visualization::g_instance->_behaviorParams = new Walk_Params(2, 3, 0.5, -5);
                    break;
                case 4:
                    Visualization::g_instance->_behaviorName = "WalkInPath";
                    break;
                case 5:
                    Visualization::g_instance->_behaviorName = "MoveHand_IK";
                    break;
                case 6:
                    Visualization::g_instance->_behaviorName = "WaveHand";
                    break;
                case 7:
                    Visualization::g_instance->_behaviorName = "Dig";
                    break;
                case 8:
                    Visualization::g_instance->_behaviorName = "Push";
//                    Visualization::g_instance->_behaviorParams = new Push_Params(0, 0);
                    break;
                case 9:
                    Visualization::g_instance->_behaviorName = "BehaviorName9";
                    break;
            }
        } else {
            switch (key) {
                case 'a':
                    cw->setHumanHeading(selectedHuman, -3.14 / 2);
                    break;
                case 's':
                    cw->setHumanHeading(selectedHuman, 3.14 / 2);
                    break;
                case 'w':
                    cw->setHumanHeading(selectedHuman, 3.14);
                    break;
                case 'z':
                    cw->setHumanHeading(selectedHuman, 0);
                    break;
                case 'r':
                    cw->reset();
                    break;
                case '-':
                    Visualization::g_instance->setHumanSpeed(Visualization::g_instance->getHumanSpeed() - 0.2);
                    cw->setHumanSpeed(selectedHuman, Visualization::g_instance->getHumanSpeed());
                    break;
                case '+':
                    Visualization::g_instance->setHumanSpeed(Visualization::g_instance->getHumanSpeed() + 0.2);
                    cw->setHumanSpeed(selectedHuman, Visualization::g_instance->getHumanSpeed());
                    break;
                case '<':
                    Visualization::g_instance->setHumanStepWidth(Visualization::g_instance->getHumanStepWidth() - 0.2);
                    cw->setHumanStepWidth(selectedHuman, Visualization::g_instance->getHumanStepWidth());
                    break;
                case '>':
                    Visualization::g_instance->setHumanStepWidth(Visualization::g_instance->getHumanStepWidth() + 0.2);
                    cw->setHumanStepWidth(selectedHuman, Visualization::g_instance->getHumanStepWidth());
                    break;
                case 'p':
                    Visualization::g_instance->throwBall(selectedHuman);
                    break;
                case 'm':
                    cw->makeHumanGrabObject(selectedHuman, "ball1", Human::left);
                    break;
                case 'n':
                    cw->makeHumanGrabObject(selectedHuman, "ball1", Human::right);
                    break;
                case 'b':
//                    cw->getHuman(selectedHuman, &human);
                    cw->makeHumanGrabObject(selectedHuman, "ball1", Human::both);
                    break;
                case 'd':
                    cw->makeHumanDropObject(selectedHuman, "ball1");
                    break;
                case 't':
                    cw->makeHumanThrowObject(selectedHuman, "ball1", Vector3d(0, 0, 15));
                    break;

//                case 'u':
//                    cw->getHuman(selectedHuman, &human);
//                    human->getBehaviour()->requestShoulderAngles(make_pair(0, 0), make_pair(0, -PI / 2), make_pair(0, PI / 2));
//                    //                    cw->setHumanPosition(selectedHuman, Vector3d(0, 1, -3.1), PI);
//                    break;
//                case 'j':
//                    cw->getHuman(selectedHuman, &human);
//                    human->getBehaviour()->requestShoulderAngles(make_pair(0, 0), make_pair(0, PI / 2), make_pair(0, -PI / 2));
//                    //                    cw->setHumanPosition(selectedHuman, Vector3d(0, 1, -3.1), 0);
//                    break;
//                case 'h':
//                    cw->getHuman(selectedHuman, &human);
//                    human->getBehaviour()->requestShoulderAngles(make_pair(0, 0), make_pair(0, PI / 2), make_pair(0, 0));
//                    //                    cw->setHumanPosition(selectedHuman, Vector3d(0, 1, -3.1), -PI/2);
//                    break;
//                case 'k':
//                    cw->getHuman(selectedHuman, &human);
//                    human->getBehaviour()->requestShoulderAngles(make_pair(0, 0), make_pair(0, 0), make_pair(0, -PI / 2));
//                    //                    cw->setHumanPosition(selectedHuman, Vector3d(0, 1, -3.1), PI/2);
//                    break;
//                case 'x':
//                    Visualization::g_instance->drawAxes();
//                    break;

                case 'u':
                    cw->resetHumanPose(selectedHuman);
                    //Visualization::g_instance->saveState(selectedHuman, true);
                    break;
                case 'i':
                    cw->setHumanPosition(selectedHuman, Vector3d(0, 1.1, -3.1), PI*0.5);
                    //Visualization::g_instance->saveState(selectedHuman, false);
                    break;
                    //                case 'j':
                    //                    Visualization::g_instance->humanTest2(selectedHuman);
                    //                    break;
                    //                case 'k':
                    //                    Visualization::g_instance->humanTest3(selectedHuman);
                    //                    break;
                default:
                    break;
            }
        }
    }

    void Visualization::initKeyboard() {
        glutKeyboardFunc(processNormalKeys);
    }

    void Visualization::saveState(string humanName, bool savestate) {
        Human* human = NULL;
        static ReducedCharacterStateArray state;

        if (_cw->getHuman(humanName, &human)) {
            Character* character = human->getCharacter();

            if (savestate) {
                state.clear();
                character->getState(&state); // Save the character state to the state array
                printf("Saving state\n");
                //                for (int i = 0; i < state.size(); i++) {
                //                    printf("%f ", state[i]);
                //                }
                //                printf("\n");
            } else if (state.size() > 0) {
                printf("Restoring state\n");
                state[0] = state[0] - 0.6;
                state[2] = state[2] - 0.6;
                state[3] = 0;
                state[4] = 0;
                state[5] = 1;
                state[6] = 0;
                human->setHeading(3.14);
                character->setState(&state); // Restore the character state to saved value
                character->setHeading(3.14);
            }

        }
    }

    typedef std::pair<double, double> LeftRightDouble;

    void Visualization::humanTest2(string humanName) {
        Human* human = NULL;
        static ReducedCharacterStateArray state;

        if (_cw->getHuman(humanName, &human)) {
            Character* character = human->getCharacter();
            SimBiController* controller = human->getController();
            CompositeController* ccontroller = human->getCompositeController();
            BehaviourController* bcontroller = human->getBehaviour();

            //            double lelbow = rand() % 314 * 0.01;
            //            double relbow = rand() % 314 * 0.01;
            //            printf("Setting elbows to %f, %f\n", lelbow, relbow);
            //            bcontroller->requestElbowBend(lelbow, relbow);
            //
            //            double knee = rand() % 100 * 0.01;
            //            printf("Setting knee bend to %f\n", knee);
            //            bcontroller->requestKneeBend(knee);

            //            double heading = rand() % 628 * 0.01;
            //            printf("Setting heading to %f\n", heading);
            //            bcontroller->requestHeading(heading);

            double lcor, rcor, lsag, rsag;
            lcor = -rand() % 3140 * 0.001;
            rcor = -rand() % 3140 * 0.001;
            lsag = rand() % 6280 * 0.001 - 3.14;
            rsag = rand() % 6280 * 0.001 - 3.14;
            ////            lsag = 0;
            ////            rsag = 0;
            LeftRightDouble twist = bcontroller->getDesiredShoulderTwist();
            printf("Setting shoulders: cor %f %f  sag %f %f  twist %f %f\n", lcor, rcor, lsag, rsag, twist.first, twist.second);
            //            //bcontroller->requestShoulderAngles(twist, make_pair(lcor, rcor), make_pair(lsag, rsag));

            //            bcontroller->setShoulderAngles(0, 0, lcor, rcor, lsag, rsag);
            bcontroller->requestShoulderAngles(make_pair(0, 0), make_pair(lcor, rcor), make_pair(0.5, 0.5));
        }
    }

    void Visualization::humanTest3(string humanName) {
        Human* human = NULL;

        if (_cw->getHuman(humanName, &human)) {
            Character* character = human->getCharacter();
            PoseController* poseC = new PoseController(character);
            human->setController((SimBiController*) poseC);
        }
    }

    //    void Visualization::humanTest1(string humanName, bool savestate) {
    //        Human* human = NULL;
    //        static ReducedCharacterStateArray state;
    //
    //        if (_cw->getHuman(humanName, &human)) {
    //            Character* character = human->getCharacter();
    //
    //            if (savestate) {
    //                state.clear();
    //                character->getState(&state); // Save the character state to the state array
    //                printf("Saving state\n");
    //                //for (int i=0; i<state.size(); i++) {
    //                //    printf("%f ", state[i]);
    //                //}
    //                //printf("\n");
    //            } else if (state.size() > 0) {
    //                printf("Restoring state\n");
    //                character->setState(&state); // Restore the character state to saved value
    //            }
    //
    //        }
    //    }
    //
    //    typedef std::pair<double, double> LeftRightDouble;
    //
    //    void Visualization::humanTest2(string humanName) {
    //        Human* human = NULL;
    //        static ReducedCharacterStateArray state;
    //
    //        if (_cw->getHuman(humanName, &human)) {
    //            Character* character = human->getCharacter();
    //            SimBiController* controller = human->getController();
    //            CompositeController* ccontroller = human->getCompositeController();
    //            BehaviourController* bcontroller = human->getBehaviour();
    //
    //            double lelbow = rand() % 314 * 0.01;
    //            double relbow = rand() % 314 * 0.01;
    //            printf("Setting elbows to %f, %f\n", lelbow, relbow);
    //            bcontroller->requestElbowBend(lelbow, relbow);
    //
    //            double knee = rand() % 100 * 0.01;
    //            printf("Setting knee bend to %f\n", knee);
    //            bcontroller->requestKneeBend(knee);
    //
    //            double heading = rand() % 628 * 0.01;
    //            printf("Setting heading to %f\n", knee);
    //            bcontroller->requestHeading(heading);
    //
    //            double lcor, rcor, lsag, rsag;
    //            lcor = -rand() % 3140 * 0.001;
    //            rcor = -rand() % 3140 * 0.001;
    //            //lsag = rand()%6280*0.001-3.14;
    //            //rsag = rand()%6280*0.001-3.14;
    //            lsag = 0;
    //            rsag = 0;
    //            LeftRightDouble twist = bcontroller->getDesiredShoulderTwist();
    //            printf("Setting shoulders: cor %f %f  sag %f %f  twist %f %f\n", lcor, rcor, lsag, rsag, twist.first, twist.second);
    //            bcontroller->requestShoulderAngles(twist, make_pair(lcor, rcor), make_pair(lsag, rsag));
    //
    //
    //        }
    //    }
    //
    //    void Visualization::humanTest3(string humanName) {
    //        Human* human = NULL;
    //        static ReducedCharacterStateArray state;
    //
    //        if (_cw->getHuman(humanName, &human)) {
    //            Character* character = human->getCharacter();
    //            PoseController* posey = new PoseController(character);
    //            human->setController((SimBiController*) posey);
    //
    //
    //
    //        }
    //    }


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
        } else if (option >= SELECT_HUMAN && option < SELECT_CONTROLLER) {
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

            if (result) {
                Visualization::g_instance->setHumanNames(humanNames);

                vector<string>::iterator itr = humanNames.begin();
                // Select the first human by default
                _selectedHumanName = (*itr);

                for (int id = SELECT_HUMAN; itr != humanNames.end(); itr++, id++) {
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
    }

    void Visualization::throwBall(string humanName) {
        Human* human = NULL;

        if (_cw->getHuman(humanName, &human)) {
            Character* character = human->getCharacter();

            double radius = rand() % 100 * 0.002 + 0.05;

            Vector3d ballScale(radius, radius, radius);
            double ballMass = rand() % 5 + radius * 8; // 1..5 kg

            Point3d ballPosition(0, 0, 0);

            double theta = rand() % 6280 * 0.001;

            Vector3d ballVelocity(15 * cos(theta), 0, 15 * sin(theta));
            double yaw = 0;
            Quaternion ballOrientation(yaw, Vector3d(0, 1, 0));

            string mesh = _cw->getPath() + "data/models/sphere10x.obj";
            Vector3d offset = Vector3d(0, 0, 0);

            RigidBody* body = new RigidBody();

            // Generate unique names
            ostringstream ostr;
            ostr << "throw_ball" << (rand() % 100 + 1);
            string name = ostr.str();

            body->setName(name.c_str());
            body->setScale(ballScale);
            body->addMeshObj(mesh.c_str(), offset, ballScale);

            double r, g, b;
            r = rand() % 800 * 0.001 + 0.2;
            g = rand() % 800 * 0.001 + 0.2;
            b = rand() % 800 * 0.001 + 0.2;
            body->setColour(r, g, b, 1);
            body->setMass(ballMass);
            body->setMOI(Vector3d(0.2, 0.2, 0.2));

            Point3d center = Point3d(0, 0, 0);
            SphereCDP* sphereCDP = new SphereCDP(center, radius, body);
            body->addCollisionDetectionPrimitive(sphereCDP);

            body->setFrictionCoefficient(1.8);
            body->setRestitutionCoefficient(0.35);

            body->setCMPosition(ballPosition);
            body->setOrientation(ballOrientation);
            body->setCMVelocity(ballVelocity);

            _cw->getWorld()->addRigidBody(body);

            ArticulatedRigidBody* torso = character->getARBByName("torso");

            Point3d parentPos = torso->getCMPosition();
            parentPos.x -= 5 * cos(theta);
            parentPos.z -= 5 * sin(theta);
            parentPos.y += rand() % 100 * 0.015 - 0.75;
            //parentPos.x -= rand()%10000*0.001 + 5.0;
            //parentPos.y -= rand()%10000*0.001 + 5.0;
            body->setCMPosition(parentPos);

            printf("Throwing ball with radius %f, mass %f from dir %f\n", radius, ballMass, theta);
        }
    }

    void Visualization::initGL(const Point3d& cameraPos, const Point3d& cameraTarget) {
        setCameraLocation(cameraPos);
        setCameraTarget(cameraTarget);

        _window = new GLUITopLevelWindow(_width, _height);
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

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glClearColor(0.0, 0.0, 0.0, 1);

        float ambient[] = {0.3f, 0.3f, 0.3f, 1.0f};
        float diffuse0[] = {0.9, 0.9, 0.9, 1.0};
        float diffuse[] = {0.6, 0.6, 0.6, 1.0};
        float specular[] = {0.0, 0.0, 0.0, 0.0};

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

        float light0_position[] = {50.0, 10.0, 200.0, 0.0};
        float light1_position[] = {200.0, 10.0, -200.0, 0.0};
        float light2_position[] = {-200.0, 10.0, -200.0, 0.0};

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
    
    void Visualization::drawPath() {
        Trajectory3d* trj = new Trajectory3d();
        trj->addKnot((1.0/3.0)*0, Point3d(0, 0.01, 0));
        trj->addKnot((1.0/3.0)*1, Point3d(1, 0.01, 0));
        trj->addKnot((1.0/3.0)*2, Point3d(-1, 0.01, 1));
        trj->addKnot((1.0/3.0)*3, Point3d(3, 0.01, 1));
        
        
        trj->setArcSize(0.6);
        
        
//        GLUtils::drawSphere(Point3d(0, 1.0, -1), 0.50, 50);
        float tempColor[] = {(float)0.6,(float)0.6,(float)0.6,(float)(1)};
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, tempColor);
//        GLUtils::drawSphere(Point3d(1.5, 1.0, -1), 0.50, 50);
        
        for (int i=0; i<trj->getKnotCount(); i++) {
            trj->getKnotPosition(i);
            Point3d p = trj->getKnotValue(i);
            GLUtils::drawSphere(p, 0.05, 20);
        } 
        
        float tempColor2[] = {(float)0.7,(float)0.7,(float)0,(float)(1)};
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, tempColor2);
        glLineWidth(2.0);
        glBegin(GL_LINE_STRIP);        
//        printf("\n\n");
        for (int i=0; i<100; i++) {
            double t = ((double)1/(double)100)*(double)i;
            Point3d p = trj->evaluate_catmull_rom(t);
//            Point3d p = trj->evaluate_linear(t);
//            printf("%f: %f, %f, %f\n", t, p.x, p.y, p.z);
            glVertex3d(p.x, p.y, p.z);
        }
//        glVertex3d(0, 1, 0);
//        glVertex3d(1, 1, 0);
//        glVertex3d(1, 1, 1);
//        glVertex3d(2, 1, 1);
        glEnd();
        
        float tempColor3[] = {(float)0,(float)0.7,(float)0,(float)(1)};
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, tempColor3);
        glBegin(GL_LINE_STRIP);
        for (int i=0; i<100; i++) {
            double t = ((double)1/(double)100)*(double)i;
            Point3d p = trj->evaluate_linear(t);
            glVertex3d(p.x, p.y, p.z);
        }
        glEnd();
    }

    void Visualization::render(CartWheel3D* cartwheel) {
        // viewport and projection matrix set-up should probably be in the reshape callback
        glViewport(0, 0, _width, _height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(_cameraFovy, _width / float(_height), 0.1, 150.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        applyCameraTransforms();

        if (_renderGround) {
            GLUtils::drawGround(50, 5, 98);
        }

        glEnable(GL_LIGHTING);

        World* world = cartwheel->getWorld();
        world->drawRBs(SHOW_COLOURS | SHOW_MESH);

        
//        GLUtils::drawSphere(Point3d(0, 0.1, -1), 0.05, 50);
//        drawPath();

        
        if (_captureImgs) {
            captureImage();
        }

        glDisable(GL_LIGHTING);

        if (_postRenderCallback) {
            _postRenderCallback();
        }
        glFlush();

        glutSwapBuffers();
    }

    void Visualization::glutStep() {
#ifdef __APPLE__
        glutCheckLoop();
#else
        glutMainLoopEvent();
#endif
    }

    void Visualization::drawAxes(void) {
        glPushMatrix();
        glLoadIdentity();
        glTranslated(-3, -2.0, -6.0);
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
            points3D.push_back(kjb::Vector(4, 1.0));
            points3D[i](0) = vertices[i].x;
            points3D[i](1) = vertices[i].y;
            points3D[i](2) = vertices[i].z;
        }

        try {
            kjb::get_projected_bbox_from_3Dpoints(bb, points3D,
                    camera.get_rendering_interface(), _width, _height);
        } catch (kjb::KJB_error e) {
            throw (e);
        }

    }
#endif

    void Visualization::mainLoop() {
        glutMainLoop();
    }

    // TODO: render the projected box overlayed onto the cartwheel image

}
