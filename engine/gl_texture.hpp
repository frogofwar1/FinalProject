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

#ifndef CW_GL_TEXTURE
#define CW_GL_TEXTURE

#include "gl_header.hpp"

namespace cw {
    
    struct Texture {
        
        GLuint tID;
        int width, height, channel;
        
        GLuint id() const { return tID; }
        
        Texture(int w, int h, int c);
        
        void init();
        
        void bind();
        void unbind();
        
        
        void update(void * data);
        
        
    };
    
}

#endif
