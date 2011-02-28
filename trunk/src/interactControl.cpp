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

static int process_option(const char* option, const char* value);

void render(void) {
	g_simulator->runStep(1.0/2000);
	g_visualization->render(g_simulator);
}

void makeWorld(CartWheel3D* p_simulator) {
	p_simulator->addObject("ground", "data/objects/flatGround.rbs", 0);

	Vector3d boxScale(0.1, 0.1, 0.3);
	double boxMass = 1.0;
	p_simulator->addBox("box1", boxScale, boxMass);

	double yaw = 3.14*0;
	Quaternion boxOrientation(yaw, Vector3d(0, 1, 0));

	Point3d boxPosition(3, 0.5, -3);
	Vector3d boxVelocity(0, 0, 0);

	p_simulator->updateRB("box1", boxPosition, boxOrientation, boxVelocity);

	const int nBalls = 5;
	for (int i = 0; i < nBalls; i++)
	{
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

#if 1
	string name = "Human1";
	string characterFile = "data/characters/bipV3.rbs";
	string controllerFile = "data/controllers/bipV3/HMV/compositeController.con";
	string actionFile = "data/controllers/bipV3/HMV/actions";
	Math::Point3d humanPosition(0.0, 1.0, 0.0);
	double heading = 3.14;

	p_simulator->addHuman(name, characterFile, controllerFile, actionFile, humanPosition, heading);
#else
	p_simulator->addHuman("data/characters/bipV3.rbs",  "data/controllers/bipV3/Walking.sbc", Point3d(0.0, 1.0, 0.0), 3.14);
#endif
}

int main(int argc, char** argv)
{
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
	cw.setPath("");

	cw.registerBuilder(makeWorld);

	viz.setCartWheelHandle(&cw);

	// Build our world
	makeWorld(&cw);
	
	viz.initMenu();
	viz.initKeyboard();
	viz.mainLoop();

	return 0;
}
