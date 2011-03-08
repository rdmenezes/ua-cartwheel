#pragma once

//#include <GL/gl.h>
//#include <GL/glu.h>

#ifdef __APPLE__
#    include <OpenGL/gl.h>
#    include <OpenGL/glu.h>
#else 
	#ifdef WIN32
	#	 include <GL/gl.h>
	#	 include <GL/glu.h>
	#else 
	#    include <GL/gl.h>       
	#    include <GL/glu.h>       
	#endif
#endif 
#include <GLUtils/GLUtilsDll.h>

namespace CartWheel { namespace GL {

/*----------------------------------------------------------------------------------------------------------------------*
 * Instances of this class will be used to easily use texture mapping (2D textures) for OpenGL applications.            *
 *----------------------------------------------------------------------------------------------------------------------*/

class GLUTILS_DECLSPEC GLTexture {
private:
	//this will act as a reference to the current texture that was created by opengl
	GLuint texID;
	//
public:
	/*
		The constructor takes as input the name of a .bmp file that contains the texture to be loaded.
		This constructor throws errors if the file cannot be found, or if it's height and width are not powers of 2
	*/
	GLTexture(const char* fileName);
	
	/*
		destructor
	*/
	~GLTexture(void);

	/*
		this method sets the current texture as the active
	*/
	void activate();

};

}
}
