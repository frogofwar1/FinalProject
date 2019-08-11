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

#ifndef  gl_mesh_INC
#define  gl_mesh_INC

#include "gl_header.hpp"
#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>


namespace cw{
    /*-----------------------------------------------------------------------------
     *  VERTEX DATA
     *-----------------------------------------------------------------------------*/
    struct Vertex{
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec4 color;
        glm::vec2 textureCoordinate;
        
        //Offsets into memory
        static size_t offsetNormal() { return sizeof(glm::vec3); }
        static size_t offsetColor() { return sizeof(glm::vec3) * 2; }
        static size_t offsetTextureCoordinate() { return sizeof(glm::vec3)*2 + sizeof(glm::vec4); }
    };
    
    struct Mesh{
        
        //position in world space
        glm::vec3 mPos;
        //orientation in world space
        glm::quat mRot;
        //scale
        float mScale;
        
        std::vector<Vertex> vertices;
        std::vector<GLushort> indices;
        
        //ID of Vertex Attribute
        GLuint positionID, normalID, colorID, textureCoordinateID;
        //A buffer ID and elementID
        GLuint bufferID, elementID;
        //An array object ID
        GLuint arrayID;
        
        
        Mesh();
        
        void getAttributes( GLuint shaderID );
        
        void buffer();
        
        void subBuffer();
        
        void bind();
        void unbind();
        
        glm::mat4 model();
        void drawArrays( GLuint mode );
        void drawElements( GLuint mode);
        
    };
    
} //cw::


#endif   /* ----- #ifndef gl_mesh_INC  ----- */
