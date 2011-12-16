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
        
        //Pushing a Light Box
        if (nOpc == 0) {
            g_simulator->doBehavior("Walk", g_visualization->getSelectedHuman().c_str(), 
                    new Walk_Params(0, 2, 0.3, 0));
            g_simulator->doBehavior("Standing", g_visualization->getSelectedHuman().c_str(), 
                    new Standing_Params(2.4, 20));
            g_simulator->doBehavior("HandShake", g_visualization->getSelectedHuman().c_str(), 
                    new HandShake_Params(0.8, 3, "Right", "Human2", "Right"));
            g_simulator->doBehavior("HandShake", g_visualization->getSelectedHuman().c_str(), 
                    new HandShake_Params(3.5, 3, "Left", "Human2", "Left"));
//            g_simulator->doBehavior("PickUp", g_visualization->getSelectedHuman().c_str(), 
//                    new PickUp_Params(1, 3, "ball1", "Left"));
//            g_simulator->doBehavior("Kick", g_visualization->getSelectedHuman().c_str(), 
//                    new Kick_Params(3, 1.2));
//            g_simulator->doBehavior("GiveObj", g_visualization->getSelectedHuman().c_str(),
//                    new GiveObj_Params(4, 2, "Human2", "ball1", "Right"));       
//            
//            g_simulator->doBehavior("Walk", g_visualization->getSelectedHuman().c_str(), 
//                    new Walk_Params(5, 3.5, -1.5, 0));
//            g_simulator->doBehavior("Catch", g_visualization->getSelectedHuman().c_str(), 
//                    new Catch_Params(8.5, 2.4, "ball1", "Left"));
//            g_simulator->doBehavior("Throw", g_visualization->getSelectedHuman().c_str(), 
//                    new Throw_Params(10, 2, "Right", "ball2", Vector3d(0,2,4.5)));
            
            
            
            g_simulator->doBehavior("Walk", "Human2", new Walk_Params(0, 2, 0.3, 3.14));
            g_simulator->doBehavior("Standing", "Human2", new Standing_Params(2.4, 20));
//            g_simulator->doBehavior("HandShake", "Human2", new HandShake_Params(1.5, 2, "Right", "Human1", "Right")); 
//            g_simulator->doBehavior("PickUp", "Human2", new PickUp_Params(0.5, 3, "ball2", "Left"));
//            g_simulator->doBehavior("GiveObj", "Human2", new GiveObj_Params(4, 2, "Human1", "ball2", "Right")); 
//            
//            g_simulator->doBehavior("Walk", "Human2", new Walk_Params(5, 3.5, 0.15, 3.14));
//            g_simulator->doBehavior("Throw", "Human2", new Throw_Params(8, 2, "Right", "ball1", Vector3d(0,1.3,-4.5)));
//            g_simulator->doBehavior("Catch", "Human2", new Catch_Params(10.5, 2.4, "ball2", "Left"));
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

//    //Adding a Box object to the scene
//    Vector3d boxScale(0.1, 0.5, 0.2);
//    double boxMass = 1.5;
//    double friction = 1.8;
//    double restitution = 0.35;
//    p_simulator->addBox("box1", boxScale, boxMass, friction, restitution);
//
//    double yaw = 3.14 * 0;
//    Quaternion boxOrientation(yaw, Vector3d(0, 1, 0));
//    Point3d boxPosition(-0.05, 1.0, -1.42);
//    Vector3d boxVelocity(0, 0, 0);
//    //Updating the position, orientation and velocity of the box object
//    p_simulator->updateRB("box1", boxPosition, boxOrientation, boxVelocity);
//
//    //Defining the position, orientation and velocity to add a ball object
//    Vector3d ballScale(0.05, 0.05, 0.05);
//    double ballMass = 0.0001;
//    Point3d ballPosition(0, 1.5, -1.45); //-2.3 or -2.6
//    Vector3d ballVelocity(0, 0, 0);
//    Quaternion ballOrientation(yaw, Vector3d(0, 1, 0));
//
//    ostringstream ostr;
//    ostr << "ball" << 1;
//    string ballName = ostr.str();
//
//    //Adding a Ball object to the scene    
//    p_simulator->addBall(ballName, ballScale, ballMass);
//    p_simulator->updateRB(ballName, ballPosition, ballOrientation, ballVelocity);
//
//    //Defining the position, orientation and velocity to add a ball2 object
//    Vector3d ballScale2(0.05, 0.05, 0.05);
//    double ballMass2 = 0.0001;
//    Point3d ballPosition2(0, 1.5, -1.3); //-2.3 or -2.6
//    Vector3d ballVelocity2(0, 0, 0);
//    Quaternion ballOrientation2(yaw, Vector3d(0, 1, 0));
//
//    ostringstream ostr2;
//    ostr2 << "ball" << 2;
//    string ballName2 = ostr2.str();
//
//    //Adding a Ball object to the scene    
//    p_simulator->addBall(ballName2, ballScale2, ballMass2, Vector3d(0.8,0,0));
//    p_simulator->updateRB(ballName2, ballPosition2, ballOrientation2, ballVelocity2);

    //Defining the human definition files and parameters
    string name = "Human1";
    string characterFile = "data/characters/bipV3.rbs";
    string controllerFile = "data/controllers/bipV3/HMV/compositeController.con";
    string actionFile = "data/controllers/bipV3/HMV/actions";
    Math::Point3d humanPosition(0, 0.95, -2.2);
    double heading = PI*0; //3.14;

    //Adding the human
    p_simulator->addHuman(name, characterFile, controllerFile, actionFile, humanPosition, heading);
    Math::Point3d humanPosition2(-0.4, 1.0, -0.25);
    p_simulator->addHuman("Human2", characterFile, controllerFile, actionFile, humanPosition2, 3.14);
}

int main(int argc, char** argv) {
    //Creating the visualization class (to show the scene and capture keys to interact with CartWheel)
    Visualization viz(render, argc, argv, 800, 600);
    g_visualization = &viz;

    // Camera Isometric
    Point3d camerPos(3.0, 2.2, 2.0);
    Point3d cameraTarget(1.0, 1.2, -1.0);
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
