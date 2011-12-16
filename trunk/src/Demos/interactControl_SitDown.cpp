#include <MathLib/Point3d.h>
#include <Core/CartWheel3D.h>
#include <Core/Human.h>
#include <Core/Visualization.h>

#include <string>
#include <sstream>

static int nOpc = 0;

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
        g_visualization->_isNewBehavior = false;
        
        //Triping from a Box
        if (nOpc == 0) {
//            g_simulator->doBehavior("Walk", g_visualization->getSelectedHuman().c_str(), 
//                    new Walk_Params(0, 1.5, 1, 0));
//            g_simulator->doBehavior("Walk", g_visualization->getSelectedHuman().c_str(), 
//                    new Walk_Params(1.5, 1, 0.7, 0));
//            g_simulator->doBehavior("Walk", g_visualization->getSelectedHuman().c_str(), 
//                    new Walk_Params(2.5, 0.5, 0, 1.5));
//            g_simulator->doBehavior("Walk", g_visualization->getSelectedHuman().c_str(), 
//                    new Walk_Params(3, 1.5, 0, 3.14));
//            g_simulator->doBehavior("Walk", g_visualization->getSelectedHuman().c_str(), 
//                    new Walk_Params(4.5, 2, -0.2, 3.14));
            g_simulator->doBehavior("SitDown", g_visualization->getSelectedHuman().c_str(), 
                    new SitDown_Params(0, 5, "box1"));
            g_simulator->doBehavior("Walk", g_visualization->getSelectedHuman().c_str(), 
                    new Walk_Params(5, 1.8, 0.15, 0));
            g_simulator->doBehavior("Standing", g_visualization->getSelectedHuman().c_str(), 
                    new Standing_Params(6.8, 20));
        }
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
    g_visualization->_isNewBehavior = true;    
    //Adding the Floor to the Simulation
    p_simulator->addObject("ground", "data/objects/flatGround.rbs", 0);

    //Adding a Box object to the scene
    Vector3d boxScale(0.4, 0.21, 0.22);
    double boxMass = 18;
    double friction = 1.8;
    double restitution = 0.35;
    p_simulator->addBox("box1", boxScale, boxMass, friction, restitution);

    double yaw = 3.14 * 0;
    Quaternion boxOrientation(yaw, Vector3d(0, 1, 0));
    Point3d boxPosition(0, 0.5, -2.42);
//    Point3d boxPosition(-0.1, 0.5, -0.3);
    Vector3d boxVelocity(0, 0, 0);
    //Updating the position, orientation and velocity of the box object
    p_simulator->updateRB("box1", boxPosition, boxOrientation, boxVelocity);

    //Defining the human definition files and parameters
    string name = "Human1";
    string characterFile = "data/characters/bipV3.rbs";
    string controllerFile = "data/controllers/bipV3/HMV/compositeController.con";
    string actionFile = "data/controllers/bipV3/HMV/actions";
    Math::Point3d humanPosition(0, 0.95, -2.1);
    double heading = PI*0; //3.14;

    //Adding the human
    p_simulator->addHuman(name, characterFile, controllerFile, actionFile, humanPosition, heading);
//    Math::Point3d humanPosition2(1, 1.0, -3.1);
//    p_simulator->addHuman("Human2", characterFile, controllerFile, actionFile, humanPosition2, heading);
}

int main(int argc, char** argv) {
    //Creating the visualization class (to show the scene and capture keys to interact with CartWheel)
    Visualization viz(render, argc, argv, 800, 600);
    g_visualization = &viz;

    // Camera Isometric
    Point3d camerPos(-6.0, 2.2, 0);
    Point3d cameraTarget(1.0, 1.2, -2.0);
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
