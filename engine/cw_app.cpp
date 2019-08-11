/*
 * =====================================================================================
 *
 *         A simple game engine for ITGT533
 *
 *         Author:  Chatchai Wangwiwattana
 *
 * =====================================================================================
 */

#include "cw_app.hpp"
#include "../imgui/imgui_impl_opengl2.h"
#include "../imgui/imgui_impl_glfw.h"

namespace cw {

    Window& App::window(){ return m_window; };
    
    App::App(int w, int h)
    {

        /*-----------------------------------------------------------------------------
        *  Initialize GLFW
        *-----------------------------------------------------------------------------*/
        if( !glfwInit() ) exit(EXIT_FAILURE);
        std::cout << ("glfw initialized \n");

        m_window.create(this, w, h, "CW Engine");
        std::cout << ("glfw window created \n");

        /*-----------------------------------------------------------------------------
         *  Initialize GLEW
         *-----------------------------------------------------------------------------*/
        GLint GlewInitResult = glewInit();
        if (GLEW_OK != GlewInitResult)
        {
            printf("ERROR: %s",glewGetErrorString(GlewInitResult));
            exit(EXIT_FAILURE);
        }
        
        /*-----------------------------------------------------------------------------
         *  Setup ImGui
         *-----------------------------------------------------------------------------*/
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        io = &ImGui::GetIO(); (void)io;
        
        ImGui::StyleColorsDark();
        
        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL( m_window.getGLFWindow(), true);
        
        ImGui_ImplOpenGL2_Init();

        /*-----------------------------------------------------------------------------
        *  Enable Alpha Blending and Depth Testing
        *-----------------------------------------------------------------------------*/
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glLineWidth(3);

    }


    /*-----------------------------------------------------------------------------
    *  Start the Game Loop
    *-----------------------------------------------------------------------------*/
    void App::start()
    {

        while ( !m_window.shouldClose() )
        {
            
            /*-----------------------------------------------------------------------------
             *  Update Game State
             *-----------------------------------------------------------------------------*/
            const float dt = systemTime.deltaTimeSecond();
            systemTime.reset();
            
            onUpdate(dt);
            
            /*-----------------------------------------------------------------------------
             *  Draw
             *-----------------------------------------------------------------------------*/
            glfwPollEvents();
   
            ImGui_ImplOpenGL2_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            
            bool show_demo_window = true;
            if (show_demo_window)
                ImGui::ShowDemoWindow(&show_demo_window);
            

            m_window.setViewport();

            glClearColor(.2,.2,.2,1);
            glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Rendering Main App
            onDraw();
            
            // Rendering UI
            ImGui::Render();
            ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

            m_window.swapBuffers();

        }

    }

    bool App::isKeyDown( int key ) const
    {
        return ( glfwGetKey(m_window.getGLFWindow(), key) == GLFW_PRESS );
    }

    /*-----------------------------------------------------------------------------
    *  Properly terminate glfw when app closes
    *-----------------------------------------------------------------------------*/
    App::~App()
    {
        glfwTerminate();
    }


}
