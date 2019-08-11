/*
 * =====================================================================================
 *
 *         A simple game engine for ITGT533
 *
 *         Author:  Chatchai Wangwiwattana
 *
 * =====================================================================================
 */

#include "gl_data.hpp"

#include "cw_header.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace cw
{
    
    Image::Image(const char* FilePath) : width(0), height(0), channel(0)
    {
        load(FilePath);
    }
    
    void Image::load(const char* filePath)
    {
        REQUIRE( filePath != nullptr);
        
        // ... process data if not NULL ...
        // ... x = width, y = height, n = # 8-bit components per pixel ...
        // ... replace '0' with '1'..'4' to force that many components per pixel
        // ... but 'n' will always be the number that it would have been if you said 0
        data = stbi_load( filePath, &width, &height, &channel, 0);
        
        PROMISE(data != nullptr);
    }
    
    Image::~Image()
    {
        stbi_image_free(data);
    }
    
} 
