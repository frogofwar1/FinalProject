/*
 * =====================================================================================
 *
 *         A simple game engine for ITGT533
 *
 *         Author:  Chatchai Wangwiwattana
 *         Adapted From: Pablo Colapinto
 *
 * =====================================================================================
 */

#ifndef  cw_app_INC
#define  cw_app_INC

#include "glfw_window.hpp"

#include "cw_time.hpp"

namespace cw {

class App{

private:
    CWTime systemTime;
    
    Window m_window;
    
    ImGuiIO* io;

public:
    Window& window();
    
    
    App(int w = 1280, int h = 720);


    /*-----------------------------------------------------------------------------
     *  Start the Draw Loop
     *-----------------------------------------------------------------------------*/
    void start();
    
    /*-----------------------------------------------------------------------------
     *  Properly terminate glfw when app closes
     *-----------------------------------------------------------------------------*/
    ~App();

    bool isUIUsingMouse() const
    {
        return io->WantCaptureMouse;
    }
    
    bool isUIUsingKeyboard() const
    {
        return io->WantCaptureKeyboard;
    }
    
    /*-----------------------------------------------------------------------------
     *  Input Events
     *-----------------------------------------------------------------------------*/
    virtual bool isKeyDown(int key) const;
    
    virtual void onUpdate( float dtSecond ) {}
    
    virtual void onDraw() {}

    virtual void onMouseMoveEvent(double x, double y){}

    virtual void onMouseEvent(int button, int action){}

    virtual void onKeyEvent(int key, int action){}
};

} //cw::


#endif   /* ----- #ifndef glut_app_INC  ----- */
