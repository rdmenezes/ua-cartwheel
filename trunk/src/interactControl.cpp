#include <MathLib/Point3d.h>
#include <Core/CartWheel3D.h>
#include <Core/Human.h>
#include <Core/Visualization.h>

#include <string>
#include <sstream>

using namespace CartWheel;
using namespace CartWheel::Core;
using namespace CartWheel::Math;

using namespace std;

static CartWheel3D* g_simulator = NULL;
static Visualization* g_visualization = NULL;

void render(void) { //Simulation loop
    //Specifying the frame rate to visualize the simulation
    const double dt = 1.0 / 2000.0;
    const double desiredFrameRate = 29.97;
    const double animationTimeToRealTimeRatio = 1.3;
    const double maxRunningTime = 0.98 / desiredFrameRate;
    double simulationTime = 0;

    //Check if there is any new call to doBehavior for CartWheel
    if (g_visualization->_isNewBehavior) {
        g_simulator->doBehavior(g_visualization->_behaviorName.c_str(),
                g_visualization->getSelectedHuman().c_str(), 
                g_visualization->_behaviorParams);
        g_visualization->_isNewBehavior = false;
    }

    //Execute each simulation step of CartWheel
    while ((simulationTime / maxRunningTime) < animationTimeToRealTimeRatio) {
        g_simulator->runStep(dt);
        simulationTime += dt;
    }
    //Show the CartWheel results on OpenGL
    g_visualization->render(g_simulator);
}

void makeWorld(CartWheel3D* p_simulator) {
    //Adding the Floor to the Simulation
    p_simulator->addObject("ground", "data/objects/flatGround.rbs", 0);

    //Adding a Box object to the scene
    Vector3d boxScale(0.1, 0.5, 0.1);
    double boxMass = 1;
//    p_simulator->addBox("box1", boxScale, boxMass);

    double yaw = 3.14 * 0;
    Quaternion boxOrientation(yaw, Vector3d(0, 1, 0));

    Point3d boxPosition(0, 1.0, -2.3);
    Vector3d boxVelocity(0, 0, 0);

    //Updating the position, orientation and velocity of the box object
//    p_simulator->updateRB("box1", boxPosition, boxOrientation, boxVelocity);

    //Defining the position, orientation and velocity to add a ball object
    Vector3d ballScale(0.05, 0.05, 0.05);
    double ballMass = 0.0001;
    Point3d ballPosition(0, 1.5, -2.3);
    Vector3d ballVelocity(0, 0, 0);
    Quaternion ballOrientation(yaw, Vector3d(0, 1, 0));

    ostringstream ostr;
    ostr << "ball" << 1;
    string ballName = ostr.str();

    //Adding a Ball object to the scene
//    p_simulator->addBall(ballName, ballScale, ballMass);
//    p_simulator->updateRB(ballName, ballPosition, ballOrientation, ballVelocity);

    /*
    const int nBalls = 5;
    for (int i = 0; i < nBalls; i++) {
            Vector3d ballScale(0.2, 0.2, 0.2);
            double ballMass = 1.0;

            ostringstream ostr;
            ostr << "ball" << i + 1;
            string name = ostr.str();

            p_simulator->addBall(name, ballScale, ballMass);

            Point3d ballPosition(-5+1.5*i, 2*0.1*(i+1), -2-1.5*i);
            Vector3d ballVelocity(0, 0, 0);
            Quaternion ballOrientation(yaw, Vector3d(0, 1, 0));

            p_simulator->updateRB(name, ballPosition, ballOrientation, ballVelocity);
    }
     */
#ifndef ADD_MANY_HUMANS

    //Defining the human definition files and parameters
    string name = "Human1";
    string characterFile = "data/characters/bipV3.rbs";
    string controllerFile = "data/controllers/bipV3/HMV/compositeController.con";
    string actionFile = "data/controllers/bipV3/HMV/actions";
    //Math::Point3d humanPosition(2.5, 1.0, -3.1);
    Math::Point3d humanPosition(0, 0.95, -3.1);
    //Math::Point3d humanPosition(2.5, 1.0, -10);
    double heading = PI*0; //3.14;

    //Adding the human
    p_simulator->addHuman(name, characterFile, controllerFile, actionFile, humanPosition, heading);
    Math::Point3d humanPosition2(1, 1.0, -3.1);
//    p_simulator->addHuman("Human2", characterFile, controllerFile, actionFile, humanPosition2, heading);

#else

    string name = "Human1";
    string characterFile = "data/characters/bipV3.rbs";
    string controllerFile = "data/controllers/bipV3/HMV/compositeController.con";
    string actionFile = "data/controllers/bipV3/HMV/actions";
    //Math::Point3d humanPosition(2.5, 1.0, -3.1);
    Math::Point3d humanPosition(0, 1.0, -3.1);
    //Math::Point3d humanPosition(2.5, 1.0, -10);
    double heading = 0; //3.14;

    p_simulator->addHuman(name, characterFile, controllerFile, actionFile, humanPosition, heading);

    string name2 = "Human2";
    string characterFile2 = "data/characters/bip3V3.rbs";
    string controllerFile2 = "data/controllers/bipV3/HMV/compositeController.con";
    string actionFile2 = "data/controllers/bipV3/HMV/actions";
    Math::Point3d humanPosition2(-3.0, 1.0, -5.0);
    double heading2 = 3.14;

    //	p_simulator->addHuman(name2, characterFile2, controllerFile2, actionFile2, humanPosition2, heading2);
    p_simulator->addHuman(name2, characterFile, "data/controllers/bipV3/SnakeWalk.sbc", humanPosition2, heading2);

    string name3 = "Human3";
    Math::Point3d humanPosition3(3.0, 1.0, -5.0);
    double heading3 = -3.14;
    p_simulator->addHuman(name3, characterFile, "data/controllers/bipV3/JumpWalk.sbc", humanPosition3, heading3);

    string name4 = "Human4";
    Math::Point3d humanPosition4(3.0, 1.0, -2.0);
    double heading4 = 3.14;
    p_simulator->addHuman(name4, characterFile, "data/controllers/bipV3/DrunkenWalk.sbc", humanPosition4, heading4);

    string name5 = "Human5";
    Math::Point3d humanPosition5(-1.0, 1.0, 1.0);
    double heading5 = -3.14 / 2;
    p_simulator->addHuman(name5, characterFile, "data/controllers/bipV3/ChickenWalk.sbc", humanPosition5, heading5);
#endif
}

int main(int argc, char** argv) {
    //Creating the visualization class (to show the scene and capture keys to interact with CartWheel)
    Visualization viz(render, argc, argv, 800, 600);
    g_visualization = &viz;
    //Point3d camerPos(1.0,1.0,-1.0);***

    //Camera Top View
//    Point3d camerPos(0.5,5.0,-2);
//    Point3d cameraTarget(0.5,1.0,-3.0);

    // Camera Close
//    Point3d camerPos(3.0,1.0,-1);
//    Point3d cameraTarget(0.0,1.0,-3.0);

    // Camera Isometric
    Point3d camerPos(0.0, 1.2, 2.0);
    Point3d cameraTarget(0.0, 1.2, -1.0);
//    Point3d camerPos(0.0, 5.0, 5.0);
//    Point3d cameraTarget(0.0, 1.0, 0.0);

    Point3d cameraUp(0.0, 1.0, 0.0);

    //Initializing and defining the Visualization parameters
    viz.initGL(camerPos, cameraTarget);
    viz.setCameraLocation(camerPos);
    viz.setCameraTarget(cameraTarget);
    viz.setCameraUp(cameraUp);

    viz.setRenderGround(true);

    //Creating the CartWheel simulator instance
    CartWheel3D cw;
    g_simulator = &cw;
    cw.setPath("");
    cw.registerBuilder(makeWorld);

    //Giving the CartWheel Instance to the Visualization object
    viz.setCartWheelHandle(&cw);

    // Build our world
    makeWorld(&cw);

    //Starting the Visualization interaction loop
    viz.initMenu();
    viz.initKeyboard();
    viz.mainLoop();

    return 0;
}
