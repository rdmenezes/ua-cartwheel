
#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include <Core/Character.h>
#include <GLUtils/GLUIWindow.h>
#include <GLUtils/GLUITopLevelWindow.h>
#include <GLUtils/GLCamera.h>

#include <MathLib/Point3d.h>

#include <string>
#include <GL/glut.h>

using CartWheel::GL::GLUITopLevelWindow;
using CartWheel::GL::GLUIWindow;
using CartWheel::Math::TransformationMatrix;
using CartWheel::Math::Point3d;

namespace CartWheel { 

class CartWheel3D;

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

    int _menuIdentifier;
    int _humanMenu;
    double _fov;
    
    CartWheel3D* _cw;
    std::string _selectedHumanName;

    TransformationMatrix _worldToCam;

    void init(int width, int height);
    void initGlut(void (*func)(void), int argc, char**argv);
    void initGLParams();

public:
    static Visualization* g_instance;

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
    void setHumanSpeed(double _humanSpeed) { this->_humanSpeed = _humanSpeed; }
    void setHumanStepWidth(double _humanStepWidth) { this->_humanStepWidth = _humanStepWidth; }
    void setCaptureImages(bool captureImgs) { _captureImgs = captureImgs; }
    void setRenderGround(bool renderGround) { _renderGround = renderGround; }
    void setDrawBB(bool drawBB) { _drawBB = drawBB; }
    void setCartWheelHandle(CartWheel3D* cw) { _cw = cw; }

    inline Point3d getCameraLocation() { return _cameraPos; }
    inline Point3d getCameraTarget() { return _cameraTarget; }
    inline double getHumanSpeed() const { return _humanSpeed; }
    inline double getHumanStepWidth() const { return _humanStepWidth; }
    inline CartWheel3D* getCartWheelHandle() const { return _cw; }

    void drawAxes();
    void render(CartWheel3D* cartwheel);

    void applyCameraTransforms();

    void captureImage();

    void selectHuman(const std::string& name) { _selectedHumanName = name; }
    std::string getSelectedHuman() const { return _selectedHumanName; }

    /**
     * Invokes glut main loop. Must be called for rendering into a glut window.
     */
    void mainLoop();
};
}

#endif // VISUALIZATION_H  
