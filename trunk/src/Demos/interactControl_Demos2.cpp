#include <MathLib/Point3d.h>
#include <Core/CartWheel3D.h>
#include <Core/Human.h>
#include <Core/Visualization.h>

#include <string>
#include <sstream>

static int nOpc = 4;

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
            g_simulator->doBehavior("Walk", g_visualization->getSelectedHuman().c_str(), 
                    new Walk_Params(0, 20, 0.1, -0.7));
        }        
        if (nOpc == 1) {
            g_simulator->doBehavior("Walk", g_visualization->getSelectedHuman().c_str(), 
                    new Walk_Params(0, 20, 0.1, 0));
        }        
        if (nOpc == 2) {
            g_simulator->doBehavior("Walk", g_visualization->getSelectedHuman().c_str(), 
                    new Walk_Params(0, 4.2, 1, 0));
        }        
        if (nOpc == 3) {
            g_simulator->doBehavior("Walk", g_visualization->getSelectedHuman().c_str(), 
                    new Walk_Params(0, 6, 1, 0));
        }        
        if (nOpc == 4) {
            g_simulator->doBehavior("Walk", g_visualization->getSelectedHuman().c_str(), 
                    new Walk_Params(0, 6, 0.9, 0));
//            g_simulator->doBehavior("Standing", g_visualization->getSelectedHuman().c_str(), 
//                    new Standing_Params(4.2, 20));
        } 
        if (nOpc == 5) {
            g_simulator->doBehavior("Walk", g_visualization->getSelectedHuman().c_str(), 
                    new Walk_Params(0, 3.1, 0.9, 0));
            g_simulator->doBehavior("Standing", g_visualization->getSelectedHuman().c_str(), 
                    new Standing_Params(3.1, 20));
        }
        
        //Parallel & Serial Behavior with Throw and Dig
        if (nOpc == 6) {            
            g_simulator->doBehavior("Walk", g_visualization->getSelectedHuman().c_str(), 
                    new Walk_Params(0, 8, 0.3, 0));
            g_simulator->doBehavior("WaveHand", g_visualization->getSelectedHuman().c_str(), 
                    new WaveHand_Params(1, 3, "Left"));
            g_simulator->doBehavior("PickUp", g_visualization->getSelectedHuman().c_str(), 
                    new PickUp_Params(2.5, 2.4, "ball1", "Right"));
            g_simulator->doBehavior("Kick", g_visualization->getSelectedHuman().c_str(), 
                    new Kick_Params(4, 1.1));
            g_simulator->doBehavior("Throw", g_visualization->getSelectedHuman().c_str(), 
                    new Throw_Params(6, 4, "Right"));
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
    Point3d humanPosition;

    //Adding a Box object to the scene
    if (nOpc>=0 && nOpc<=2) {
        Vector3d boxScale(0.15, 0.1, 0.11);
        double boxMass = 0.00001; //0.001; //2, 0.3, 0.001
        p_simulator->addBox("box1", boxScale, boxMass);
        double yaw = 3.14 * 0;
        Quaternion boxOrientation(yaw, Vector3d(0, 1, 0));
        Point3d boxPosition(-0.1, 1.0, -1.5);
        Vector3d boxVelocity(0, 0, 0);
        //Updating the position, orientation and velocity of the box object
        p_simulator->updateRB("box1", boxPosition, boxOrientation, boxVelocity);
    }
    if (nOpc==3) {
        Vector3d boxScale(0.15, 0.1, 0.11);
        double boxMass = 1; //0.001; //2, 0.3, 0.001
        p_simulator->addBox("box1", boxScale, boxMass);
        double yaw = 3.14 * 0;
        Quaternion boxOrientation(yaw, Vector3d(0, 1, 0));
        Point3d boxPosition(-0.25, 1.0, -1.3);
        Vector3d boxVelocity(0, 0, 0);
        //Updating the position, orientation and velocity of the box object
        p_simulator->updateRB("box1", boxPosition, boxOrientation, boxVelocity);
    }
    if (nOpc==4) {
        Vector3d boxScale(0.14, 0.09, 0.09);
        double boxMass = 15; //0.001; //2, 0.3, 0.001
        p_simulator->addBox("box1", boxScale, boxMass);
        double yaw = 3.14 * 0;
        Quaternion boxOrientation(yaw, Vector3d(0, 1, 0));
        Point3d boxPosition(-0.18, 1.0, -1.3);
        Vector3d boxVelocity(0, 0, 0);
        //Updating the position, orientation and velocity of the box object
        p_simulator->updateRB("box1", boxPosition, boxOrientation, boxVelocity);
    }
    if (nOpc==5) {
        Vector3d boxScale(0.14, 0.09, 0.09);
        double boxMass = 36; //0.001; //2, 0.3, 0.001
        p_simulator->addBox("box1", boxScale, boxMass);
        double yaw = 3.14 * 0;
        Quaternion boxOrientation(yaw, Vector3d(0, 1, 0));
        Point3d boxPosition(-0.1, 1.0, -2);
        Vector3d boxVelocity(0, 0, 0);
        //Updating the position, orientation and velocity of the box object
        p_simulator->updateRB("box1", boxPosition, boxOrientation, boxVelocity);
    }
    if (nOpc==6) {
        Vector3d boxScale(0.1, 0.5, 0.1);
        double boxMass = 0.8;
        p_simulator->addBox("box1", boxScale, boxMass);
        double yaw = 3.14 * 0;
        Quaternion boxOrientation(yaw, Vector3d(0, 1, 0));
        Point3d boxPosition(-0.05, 1.0, -1.42);
        Vector3d boxVelocity(0, 0, 0);
        //Updating the position, orientation and velocity of the box object
        p_simulator->updateRB("box1", boxPosition, boxOrientation, boxVelocity);
        
        //Defining the position, orientation and velocity to add a ball object
        Vector3d ballScale(0.05, 0.05, 0.05);
        double ballMass = 0.0001;
        Point3d ballPosition(-0.14, 1.5, -1.509); //-2.3 or -2.6
        Vector3d ballVelocity(0, 0, 0);
        Quaternion ballOrientation(yaw, Vector3d(0, 1, 0));

        ostringstream ostr;
        ostr << "ball" << 1;
        string ballName = ostr.str();

        //Adding a Ball object to the scene    
        p_simulator->addBall(ballName, ballScale, ballMass);
        p_simulator->updateRB(ballName, ballPosition, ballOrientation, ballVelocity);
    }

    
    //Defining the human definition files and parameters
    string name = "Human1";
    string characterFile = "data/characters/bipV3.rbs";
    string controllerFile = "data/controllers/bipV3/HMV/compositeController.con";
    string actionFile = "data/controllers/bipV3/HMV/actions";
    
    if (nOpc <= 5) {
        Math::Point3d humanPosition(0, 0.95, -3.6);
        double heading = PI*0; //3.14;
        //Adding the human
        p_simulator->addHuman(name, characterFile, controllerFile, actionFile, humanPosition, heading);
    } else {
        Math::Point3d humanPosition(0, 0.95, -3.1);
        double heading = PI*0; //3.14;
        //Adding the human
        p_simulator->addHuman(name, characterFile, controllerFile, actionFile, humanPosition, heading);
    } 
}

int main(int argc, char** argv) {
    //Creating the visualization class (to show the scene and capture keys to interact with CartWheel)
    Visualization viz(render, argc, argv, 800, 600);
    g_visualization = &viz;    

    // Camera
    Point3d camerPos;
    Point3d cameraTarget;
    Point3d cameraUp;
    
    if (nOpc < 5) {
        // Camera
        camerPos = Point3d(3.0, 1.1, -1.0);
        cameraTarget = Point3d(1.0, 0.9, -1.0);
        cameraUp = Point3d(0.0, 1.0, 0.0);
    } else if(nOpc==5) {        
        // Camera
        camerPos = Point3d(-4.4, 1.1, -2.7);
        cameraTarget = Point3d(1.0, 0.9, -2.0);
        cameraUp = Point3d(0.0, 1.0, 0.0);
    } else {
        // Camera Isometric
        camerPos = Point3d(3.0, 2.2, 2.0);
        cameraTarget = Point3d(1.0, 1.2, -1.0);
        cameraUp = Point3d(0.0, 1.0, 0.0);
    }

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
