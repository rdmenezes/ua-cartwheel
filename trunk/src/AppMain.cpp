#include <Core/CartWheel3D.h>

#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <algorithm>

using std::max;
using std::min;

using std::vector;
using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;

using namespace CartWheel;
using namespace CartWheel::Core;
using namespace CartWheel::Math;

CartWheel3D* simulator;
char* sPath;

void init_simulation(int argc, char** argv) {
	simulator = new CartWheel3D(sPath);
	simulator->addObject("ground", "data/objects/flatGround.rbs", -1);

	simulator->addObject("dodgeBox", "data/objects/box.rbs", 1);

	string humanModel = "data/characters/bipV3.rbs";
	string humanController = "data/controllers/bipV3/Walking.sbc";

	simulator->addHuman(humanModel, humanController, Point3d(0.0, 1.0, -1.0), 0);
	simulator->setHumanSpeed(0, 1.0);
//	simulator->setHumanHeading(0, 3.14);
}

int main(int argc, char** argv) {
	FILE* option_fp;

	sPath = new char[200];
	strcpy(sPath, "");

	init_simulation(argc, argv);
	for (int timer = 0; timer < 10000; timer++) {
		simulator->runStep(0.001);
		cout << "human location : "
			 << simulator->getHumanPosition(0).getX() << "  "
			 << simulator->getHumanPosition(0).getZ() << endl;
	}
}
