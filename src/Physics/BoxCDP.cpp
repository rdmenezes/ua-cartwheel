#include <Physics/BoxCDP.h>
#include <GLUtils/GLUtils.h>

using namespace CartWheel;
using namespace CartWheel::GL;
using namespace CartWheel::Physics;
using namespace CartWheel::Util;

BoxCDP::~BoxCDP(void){

}

/**
	Draw an outline of the box
*/
void BoxCDP::draw(){
	GLUtils::drawBox(p1, p2);
}

