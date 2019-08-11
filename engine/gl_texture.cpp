/*
 * =====================================================================================
 *
 *         A simple game engine for ITGT533
 *
 *         Author:  Chatchai Wangwiwattana
 *
 * =====================================================================================
 */

#include "gl_texture.hpp"
#include "cw_header.hpp"

namespace cw
{
    
    Texture::Texture(int w, int h, int c) : width(w), height(h), channel(c)
    {
        init();
    };
    
    
    void Texture::init()
    {
        
        /*-----------------------------------------------------------------------------
         *  Generate Texture and Bind it
         *-----------------------------------------------------------------------------*/
        glGenTextures(1, &tID);
        glBindTexture(GL_TEXTURE_2D, tID);
        
        /*-----------------------------------------------------------------------------
         *  Allocate Memory on the GPU
         *-----------------------------------------------------------------------------*/
        // target | lod | internal_format | width | height | border | format | type | data
        
        if(channel == 3)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        }
        
        
        /*-----------------------------------------------------------------------------
         *  Set Texture Parameters
         *-----------------------------------------------------------------------------*/
        // Set these parameters to avoid a black screen caused by improperly mipmapped textures
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        //optional . . .
        //
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        /*-----------------------------------------------------------------------------
         *  Unbind texture
         *-----------------------------------------------------------------------------*/
        glBindTexture(GL_TEXTURE_2D, 0);
        
    }
    
    void Texture::bind()
    {
        glBindTexture(GL_TEXTURE_2D, tID);
        
    }
    
    void Texture::unbind()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
        
    }
    
    void Texture::update(void * data)
    {
        REQUIRE( data != nullptr);
        
        bind();
        /*-----------------------------------------------------------------------------
         *  Load data onto GPU
         *-----------------------------------------------------------------------------*/
        // target | lod | xoffset | yoffset | width | height | format | type | data
        
        if(channel == 3)
        {
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data );
        }
        else
        {
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data );
        }
        
        
        glGenerateMipmap(GL_TEXTURE_2D);
        
        unbind();
        
    }
    
    
}
