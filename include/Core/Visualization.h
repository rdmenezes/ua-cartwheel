
#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include <Core/Character.h>
#include <GLUtils/GLUIWindow.h>
#include <GLUtils/GLUITopLevelWindow.h>
#include <GLUtils/GLCamera.h>
#include "Behaviors/Params.h"

#include <MathLib/Point3d.h>
#include <MathLib/Trajectory.h>

#include <string>
#include <list>

#include <boost/shared_ptr.hpp>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

using CartWheel::GL::GLUITopLevelWindow;
using CartWheel::GL::GLUIWindow;
using CartWheel::Math::TransformationMatrix;
using CartWheel::Math::Point3d;

namespace CartWheel { 

class CartWheel3D;
class Visualization;

typedef boost::shared_ptr<Visualization> VisualizationPtr;

class Visualization {

protected:
    GLUITopLevelWindow* _window;
    GLUIWindow* _mainWindow;

    int _width;
    int _height;

    double _humanSpeed;
    double _humanStepWidth;

    bool _renderGround;
    bool _captureImgs;
    bool _drawBB;

    Point3d _cameraPos;
    Point3d _cameraTarget;
    Point3d _cameraUp;
    double _cameraFovy;

    int _menuIdentifier;
    int _humanMenu;
    double _fov;
    
    CartWheel3D* _cw;
    std::string _selectedHumanName;
    std::vector<std::string> _humanNames;

    TransformationMatrix _worldToCam;

    void init(int width, int height);
    void initGlut(void (*func)(void), int argc, char**argv);
    void initGLParams();

    void (*_postRenderCallback)(void);

public:
    static Visualization* g_instance;
    bool _isNewBehavior;
    std::string _behaviorName;
    Behaviors::Params* _behaviorParams;

public:

    /** The no parameter constructor will initialize an off
     * screen rendering context (hopefully an image)
     */
    Visualization(int width, int height);

    /**
     * This constructor will initialize a GLUT window to handle any rendering.
     */
    Visualization(void (*func)(void), int argc, char** argv, 
            int width, int height);

    ~Visualization();

    void initGL(const Point3d& cameraPos, const Point3d& cameraTarget);

    /**
     * Enable right-button click activated menu
     */
    void initMenu();

    /**
     * Enable keyboard key
     */
    void initKeyboard();

    void setCameraLocation(const Point3d& pos) { _cameraPos = pos; }
    void setCameraTarget(const Point3d& target) { _cameraTarget = target; }
    void setCameraUp(const Point3d& up) { _cameraUp = up; }
    void setCameraFovy(int fovy) { _cameraFovy = fovy; }
    void setHumanSpeed(double _humanSpeed) { this->_humanSpeed = _humanSpeed; }
    void setHumanStepWidth(double _humanStepWidth) { this->_humanStepWidth = _humanStepWidth; }
    void setCaptureImages(bool captureImgs) { _captureImgs = captureImgs; }
    void setRenderGround(bool renderGround) { _renderGround = renderGround; }
    void setDrawBB(bool drawBB) { _drawBB = drawBB; }
    void setCartWheelHandle(CartWheel3D* cw) { _cw = cw; }
    void setWidth(int width) { _width = width; glutReshapeWindow(_width, _height); }
    void setHeight(int height) { _height = height; glutReshapeWindow(_width, _height); }
    void setHumanNames(const std::vector<std::string>& humanNames) { _humanNames = humanNames; }

    int getWidth() { return _width; }
    int getHeight() { return _height; }
    Point3d getCameraLocation() { return _cameraPos; }
    Point3d getCameraTarget() { return _cameraTarget; }
    double getCameraFovy() { return _cameraFovy; }
    double getHumanSpeed() const { return _humanSpeed; }
    double getHumanStepWidth() const { return _humanStepWidth; }
    CartWheel3D* getCartWheelHandle() const { return _cw; }
    void getHumanNames(std::vector<std::string>& humanNames) const { humanNames = _humanNames; }

    void throwBall(std::string humanName);

    void drawAxes();
    void render(CartWheel3D* cartwheel);

    /// useful for drawing user-defined overlays like bounding boxes, etc
    void setPostRenderCallback(void (*cb)()) { _postRenderCallback = cb; }

    void applyCameraTransforms();

    void captureImage();

    void selectHuman(const std::string& name) { _selectedHumanName = name; }
    std::string getSelectedHuman() const { return _selectedHumanName; }

    void saveState(std::string humanName, bool savestate);
    void humanTest2(std::string humanName);
    void humanTest3(std::string humanName);
    void drawPath();

    void glutStep();

    /**
     * Invokes glut main loop. Must be called for rendering into a glut window.
     */
    void mainLoop();
};
}

#endif // VISUALIZATION_H  
