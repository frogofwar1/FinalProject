/*
 * =====================================================================================
 *
 *         A simple game engine for ITGT533
 *
 *         Author:  Chatchai Wangwiwattana
 *
 * =====================================================================================
 */

#ifndef  gl_data_INC
#define  gl_data_INC

#include <vector>

namespace cw {

    struct Image{

        int width;
        int height;
        int channel;

        unsigned char *data;
        
        Image(const char* FilePath);
        void load(const char* FilePath);
        ~Image();
    };

} 

#endif
