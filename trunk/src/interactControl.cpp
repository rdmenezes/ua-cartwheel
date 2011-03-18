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

void render(void) {
	g_simulator->runStep(1.0/2000);
	g_visualization->render(g_simulator);
}

void makeWorld(CartWheel3D* p_simulator) {
	p_simulator->addObject("ground", "data/objects/flatGround.rbs", 0);

	Vector3d boxScale(0.2, 0.2, 0.3);
	double boxMass = 1.0;
	p_simulator->addBox("box1", boxScale, boxMass);

	double yaw = 3.14*0;
	Quaternion boxOrientation(yaw, Vector3d(0, 1, 0));

	Point3d boxPosition(3, 1.0, -3);
	Vector3d boxVelocity(0, 0, 0);

	//p_simulator->updateRB("box1", boxPosition, boxOrientation, boxVelocity);

	Vector3d ballScale(0.2, 0.2, 0.2);
	double ballMass = 1.0;

	ostringstream ostr;
	ostr << "ball" << 1;
	string ballName = ostr.str();

	p_simulator->addBall(ballName, ballScale, ballMass);

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

	string name = "Human1";
	string characterFile = "data/characters/bipV3.rbs";
	string controllerFile = "data/controllers/bipV3/HMV/compositeController.con";
	string actionFile = "data/controllers/bipV3/HMV/actions";
	//Math::Point3d humanPosition(2.5, 1.0, -3.1);
	Math::Point3d humanPosition(0, 1.0, -3.1);
	//Math::Point3d humanPosition(2.5, 1.0, -10);
	double heading = 0;//3.14;

	p_simulator->addHuman(name, characterFile, controllerFile, actionFile, humanPosition, heading);

#else

	string name = "Human1";
	string characterFile = "data/characters/bipV3.rbs";
	string controllerFile = "data/controllers/bipV3/HMV/compositeController.con";
	string actionFile = "data/controllers/bipV3/HMV/actions";
	//Math::Point3d humanPosition(2.5, 1.0, -3.1);
	Math::Point3d humanPosition(0, 1.0, -3.1);
	//Math::Point3d humanPosition(2.5, 1.0, -10);
	double heading = 0;//3.14;

	p_simulator->addHuman(name, characterFile, controllerFile, actionFile, humanPosition, heading);

	string name2 = "Human2";
	string characterFile2 = "data/characters/bip3V3.rbs";
	string controllerFile2 = "data/controllers/bipV3/HMV/compositeController.con";
	string actionFile2 = "data/controllers/bipV3/HMV/actions";
	Math::Point3d humanPosition2(-3.0, 1.0, -5.0);
	double heading2 = 3.14;

//	p_simulator->addHuman(name2, characterFile2, controllerFile2, actionFile2, humanPosition2, heading2);
	p_simulator->addHuman(name2, characterFile,  "data/controllers/bipV3/SnakeWalk.sbc", humanPosition2, heading2);

	string name3 = "Human3";
	Math::Point3d humanPosition3(3.0, 1.0, -5.0);
	double heading3 = -3.14;
	p_simulator->addHuman(name3, characterFile,  "data/controllers/bipV3/JumpWalk.sbc", humanPosition3, heading3);

	string name4 = "Human4";
	Math::Point3d humanPosition4(3.0, 1.0, -2.0);
	double heading4 = 3.14;
	p_simulator->addHuman(name4, characterFile,  "data/controllers/bipV3/DrunkenWalk.sbc", humanPosition4, heading4);

	string name5 = "Human5";
	Math::Point3d humanPosition5(-1.0, 1.0, 1.0);
	double heading5 = -3.14/2;
	p_simulator->addHuman(name5, characterFile,  "data/controllers/bipV3/ChickenWalk.sbc", humanPosition5, heading5);
#endif
}

int main(int argc, char** argv)
{
    Visualization viz(render, argc, argv, 800, 600);
    g_visualization = &viz;
    Point3d camerPos(1.0,5.0,1.0);
    Point3d cameraTarget(1.0,1.0,-5.0);
    //Point3d camerPos(0.0,5.0,5.0);
    //Point3d cameraTarget(0.0,1.0,0.0);
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
