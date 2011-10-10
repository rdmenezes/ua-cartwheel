#include <Core/SimGlobals.h>
#include <limits>

using namespace CartWheel;
using namespace CartWheel::Core;
using namespace CartWheel::Physics;
using namespace CartWheel::Math;
using namespace CartWheel::Util;

//initialize all the parameters to some sensible values.

//give this a very high value so that we can use the scripted values in the rb specs for the value to use
int SimGlobals::forceHeadingControl = false;
double SimGlobals::desiredHeading = 0;
int SimGlobals::steps_per_second = 2000;
double SimGlobals::dt = 1.0/((double)SimGlobals::steps_per_second);
World* SimGlobals::activeRbEngine = NULL;

double SimGlobals::conInterpolationValue;
double SimGlobals::bipDesiredVelocity;


double SimGlobals::nan = std::numeric_limits<double>::quiet_NaN();

double SimGlobals::targetPos = 0;


double SimGlobals::targetPosX = 0;
double SimGlobals::targetPosZ = 0;

int SimGlobals::constraintSoftness = 1;
int SimGlobals::CGIterCount = 0;
int SimGlobals::linearizationCount = 1;

double SimGlobals::rootSagittal = 0;
double SimGlobals::stanceKnee = 0;
/*

double SimGlobals::style = 0;

double SimGlobals::rootLateral = 0;
double SimGlobals::swingHipSagittal = 0;
double SimGlobals::swingHipLateral = 0;
double SimGlobals::stanceAngleSagittal = 0;
double SimGlobals::stanceAngleLateral = 0;

double SimGlobals::VDelSagittal = 0;
double SimGlobals::stepHeight = 0;
double SimGlobals::stepTime = 0.6;
double SimGlobals::duckWalk = 0;
double SimGlobals::upperBodyTwist = 0;
double SimGlobals::coronalStepWidth = 0.1;


double SimGlobals::COMOffsetX = 0;
double SimGlobals::COMOffsetZ = 0;
*/

