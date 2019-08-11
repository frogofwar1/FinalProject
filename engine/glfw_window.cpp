/*
 * =====================================================================================
 *
 *         A simple game engine for ITGT533
 *
 *         Author:  Chatchai Wangwiwattana
 *
 * =====================================================================================
 */

#include "glfw_window.hpp"

#include "gl_header.hpp"
#include <iostream>


using namespace std;

namespace cw{
    
    void * Interface::app;
    
    int Window::width()  { return m_width; }
    int Window::height() { return m_height; }
    float Window::ratio() { return (float)m_width/m_height;}
    
    Window::Window() {}
    
    
    //Get the Current framebuffer Size in pixels and Set the Viewport to it
    void Window::setViewport(){
        glfwGetFramebufferSize(window, &m_width, &m_height);
        glViewport(0,0,m_width,m_height);
    }
    
    //Check whether window should close
    bool Window::shouldClose(){
        return glfwWindowShouldClose(window);
    }
    
    //Swap front and back buffers
    void Window::swapBuffers(){
        glfwSwapBuffers(window);
    }
    
    //Destroy the window
    void Window::destroy(){
        glfwDestroyWindow(window);
    }
    
    Window::~Window(){
        destroy();
    }
    
    
} 

