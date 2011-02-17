#pragma once

#include <GLUtils/GLUtilsDll.h>

namespace CartWheel { namespace GL {

class GLUTILS_DECLSPEC GLUICallback {
public:
	GLUICallback() {}
	virtual ~GLUICallback() {}
	virtual void execute() {}

};

}
}
