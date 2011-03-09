#ifndef _CONTROLUTILS_H_
#define _CONTROLUTILS_H_

#include <MathLib/Vector3d.h>
#include <math.h>

namespace CartWheel
{
using CartWheel::Math::Vector3d;

class ControlUtils{

public:

static double eucDistance(const Vector3d & a, const Vector3d & b){
        return sqrt((a -b).dotProductWith((a - b)));
}

static double eucDistance2d(const Vector3d & a, const Vector3d & b){
        return sqrt(( a.getX() - b.getX()) * ( a.getX() - b.getX()) + ( a.getZ() - b.getZ()) * ( a.getZ() - b.getZ())  );
}

double static gaussianPD(double m, double s, double t) {
  double pi = 3.1415926;
  return (1.0 / sqrt(2 * pi * pow(s,2))) * exp(-pow(t-m,2) / (2 * pow(s,2)));
}

};

} // namespace CartWheel
#endif
