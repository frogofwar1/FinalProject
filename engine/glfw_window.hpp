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

#ifndef cw_WINDOW_H_INCLUDED
#define cw_WINDOW_H_INCLUDED

#include "gl_header.hpp"
#include <iostream>

#include "../imgui/imgui.h"


namespace cw
{
    
    /*-----------------------------------------------------------------------------
     *  Some Callbacks to be implemented later
     *-----------------------------------------------------------------------------*/
    struct Interface {
        
        static void * app;
        
        template<class APPLICATION>
        static void OnKeyEvent(GLFWwindow* window, int key, int scancode, int action, int mods){
            
            if( !((APPLICATION*)(app))->isUIUsingKeyboard())
            {
                ((APPLICATION*)(app))->onKeyEvent(key,action);
            }
        }
        
        template<class APPLICATION>
        static void OnMouseEvent(GLFWwindow* window, int button, int action, int mods){
            
            if( !((APPLICATION*)(app))->isUIUsingMouse())
            {
                ((APPLICATION*)(app))->onMouseEvent(button,action);
            }
            
        }
        
        template<class APPLICATION>
        static void OnMouseMoveEvent(GLFWwindow* window, double x, double y){
            
            if( !((APPLICATION*)(app))->isUIUsingMouse())
            {
                ((APPLICATION*)(app))->onMouseMoveEvent(x,y);
            }
        }
    
        
    };
    
    
    
    /*-----------------------------------------------------------------------------
     *  A GLFW Window Wrapper
     *-----------------------------------------------------------------------------*/
    class Window final
    {
        
    private:
        
        GLFWwindow * window;
        Interface interface;
        
        int m_width;
        int m_height;
        
    public:
        Window();
        Window(const Window&) = default;
        ~Window();
        
        // getter setter
        int width();
        int height();
        float ratio();
        
        
        GLFWwindow * getGLFWindow() const {
            return window;
        };
        
        /*! @brief Get the Current framebuffer Size in pixels and Set the Viewport to it.*/
        void setViewport();
        
        /*! @brief Check whether window should close. */
        bool shouldClose();
        
        /*! @brief Swap front and back buffers */
        void swapBuffers();
        
        /*! @brief Destroy the window */
        void destroy();
        
        
        /*-----------------------------------------------------------------------------
         *  TEMPLATE
         *-----------------------------------------------------------------------------*/
        
        /*! @brief Create a Window Context */
        template<class APPLICATION>
        void create(APPLICATION * app, int w, int h, const char * name="demo")
        {
            interface.app = app;
            
            m_width = w;
            m_height = h;
            
            window = glfwCreateWindow(w,h,name,NULL ,NULL);
            if (!window) {
                glfwTerminate();
                exit(EXIT_FAILURE);
            }
            glfwMakeContextCurrent(window);
            glfwSwapInterval(1);

            
            //register callbacks for keyboard and mouse
            glfwSetKeyCallback(window, Interface::OnKeyEvent<APPLICATION>);
            glfwSetCursorPosCallback(window, Interface::OnMouseMoveEvent<APPLICATION> );
            glfwSetMouseButtonCallback(window, Interface::OnMouseEvent<APPLICATION> );
            
        }
        
    };
    
} //cw

#endif
