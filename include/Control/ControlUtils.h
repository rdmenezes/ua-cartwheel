#ifndef _CONTROLUTILS_H_
#define _CONTROLUTILS_H_

#include <MathLib/Vector3d.h>

using CartWheel::Math::Vector3d;

class ControlUtils{

public:

static double eucDistance(const Vector3d & a, const Vector3d & b){
        return sqrt((a -b).dotProductWith((a - b)));
}

static double eucDistance2d(const Vector3d & a, const Vector3d & b){
        return sqrt(( a.getX() - b.getX()) * ( a.getX() - b.getX()) + ( a.getZ() - b.getZ()) * ( a.getZ() - b.getZ())  );
}

};

#endif
