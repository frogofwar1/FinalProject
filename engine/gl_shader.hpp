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

#ifndef CW_GL_SHADER
#define CW_GL_SHADER

#include "gl_header.hpp"

namespace cw {
    
    class Shader {
        
        GLuint sID;
        
    public:
        
        GLuint id() const { return sID; }
        
        Shader(const char * vert, const char * frag);
        
        void bind();
        void unbind();
        
        
        /*-----------------------------------------------------------------------------
         *  FUNCION TO CHECK FOR SHADER COMPILER ERRORS
         *-----------------------------------------------------------------------------*/
        void compilerCheck(GLuint ID);
        
        
        /*-----------------------------------------------------------------------------
         *  FUNCION TO CHECK FOR SHADER LINK ERRORS
         *-----------------------------------------------------------------------------*/
        void linkCheck(GLuint ID);
        
    };
    
    
    
} //cw::

#endif
