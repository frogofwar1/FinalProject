/*
 * =====================================================================================
 *
 *         A simple game engine for ITGT533
 *
 *         Author:  Chatchai Wangwiwattana
 *
 * =====================================================================================
 */

#include "gl_mesh.hpp"
#include "gl_macros.hpp"

namespace cw{

    
    Mesh::Mesh() : mScale(1), mRot(1,0,0,0), mPos(0,0,0) {}
    
    void Mesh::getAttributes( GLuint shaderID ){
        // Get attribute locations from SHADER (if these attributes do not exist in shader, ID=-1)
        positionID = glGetAttribLocation(shaderID, "position");
        colorID = glGetAttribLocation(shaderID, "color");
        normalID = glGetAttribLocation(shaderID, "normal");
        textureCoordinateID = glGetAttribLocation(shaderID, "textureCoordinate");
    }
    
    void Mesh::buffer(){
        
        /*-----------------------------------------------------------------------------
         *  CREATE THE VERTEX ARRAY OBJECT
         *-----------------------------------------------------------------------------*/
        GENVERTEXARRAYS(1, &arrayID);
        BINDVERTEXARRAY(arrayID);
        
        /*-----------------------------------------------------------------------------
         *  CREATE THE VERTEX BUFFER OBJECT
         *-----------------------------------------------------------------------------*/
        // Generate one buffer
        glGenBuffers(1, &bufferID);
        glBindBuffer( GL_ARRAY_BUFFER, bufferID );
        glBufferData( GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &(vertices[0]), GL_DYNAMIC_DRAW );
        
        /*-----------------------------------------------------------------------------
         *  CREATE THE ELEMENT ARRAY BUFFER OBJECT
         *-----------------------------------------------------------------------------*/
        glGenBuffers(1, &elementID);
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, elementID );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), &(indices[0]), GL_STATIC_DRAW );
        
        /*-----------------------------------------------------------------------------
         *  ENABLE VERTEX ATTRIBUTES
         *-----------------------------------------------------------------------------*/
        glEnableVertexAttribArray(positionID);
        glEnableVertexAttribArray(colorID);
        glEnableVertexAttribArray(normalID);
        glEnableVertexAttribArray(textureCoordinateID);
        
        // Tell OpenGL how to handle the buffer of data that is already on the GPU
        //                      attrib    num   type     normalize   stride     offset
        glVertexAttribPointer( positionID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0 );
        glVertexAttribPointer( colorID, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) Vertex::offsetColor() );
        glVertexAttribPointer( normalID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) Vertex::offsetNormal() );
        glVertexAttribPointer( textureCoordinateID, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) Vertex::offsetTextureCoordinate() );
        
        // Unbind Everything (NOTE: unbind the vertex array object first)
        BINDVERTEXARRAY(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    
    void Mesh::subBuffer(){
        bind();
        glBindBuffer(GL_ARRAY_BUFFER, bufferID);
        glBufferSubData( GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data() );
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        unbind();
    } 
    
    void Mesh::bind(){ BINDVERTEXARRAY(arrayID); }
    void Mesh::unbind() { BINDVERTEXARRAY(0); }
    
    glm::mat4 Mesh::model()
    {
        glm::mat4 scale = glm::scale( glm::mat4(1), glm::vec3(mScale) );
        glm::mat4 rotate = glm::toMat4( mRot );
        glm::mat4 translate = glm::translate( glm::mat4(1), mPos );
        
        return translate * rotate * scale;
    }
    
    void Mesh::drawArrays( GLuint mode )
    {
        glDrawArrays( mode, 0, static_cast<GLsizei>(vertices.size()) );
        
    }
    void Mesh::drawElements( GLuint mode)
    {
        glDrawElements( mode, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_SHORT, 0 );
        
    }
    
    
} //cw::
