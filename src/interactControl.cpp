#include <MathLib/Point3d.h>
#include <Core/CartWheel3D.h>
#include <Core/Human.h>
#include <Core/Visualization.h>

#include <string>

using namespace CartWheel;
using namespace CartWheel::Core;
using namespace CartWheel::Math;

using namespace std;

static CartWheel3D* g_simulator = NULL;
static Visualization* g_visualization = NULL;

string g_simulatorPath;

static int process_option(const char* option, const char* value);

void render(void) {
	g_simulator->runStep(1.0/2000);
	g_visualization->render(g_simulator);
}

int main(int argc, char** argv) { 
	
    Visualization viz(render, argc, argv, 640, 480);
    g_visualization = &viz;
    Point3d camerPos(0.0,5.0,5.0);
    Point3d cameraTarget(0.0,1.0,0.0);
    Point3d cameraUp(0.0,1.0,0.0);

    viz.initGL(camerPos, cameraTarget);
    viz.setCameraLocation(camerPos);
    viz.setCameraTarget(cameraTarget);
    viz.setCameraUp(cameraUp);

    viz.setRenderGround(true);

    CartWheel3D cw;
    g_simulator = &cw;
	cw.setPath(g_simulatorPath);
	cw.addObject("ground", "data/objects/flatGround.rbs", 0);

	viz.setCartWheelHandle(&cw);

	//cw.setDrawBB(false);
	
	Vector3d scale(0.1, 0.1, 0.3);
	double mass = 1.0;
//	cw.addBox("box1", scale, mass);

	double yaw = 3.14*0.2;
	Quaternion orientation(yaw, Vector3d(0, 1, 0));

	Point3d position(0.5,0.5,-2);
	Vector3d velocity(0, 0, 0);

	//cw.updateRB("box1", position, orientation, velocity);
#if 1
	cw.addHuman("data/characters/bipV3.rbs",  "data/controllers/bipV3/Walking.sbc", Point3d(0.0, 1.0, 0.0), 3.14);
#else
	cw.addHuman("data/characters/bigBird.rbs",  "data/controllers/bigBird/RL/iWalk.sbc", Point3d(0.0, 1.0, 0.0), 0);
#endif
	
	viz.initMenu();
	viz.initKeyboard();
	viz.mainLoop();

	return 0;
}
