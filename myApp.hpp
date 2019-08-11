/*
 * =====================================================================================
 *
 *         A simple game engine for ITGT533
 *
 *         Author:  Chatchai Wangwiwattana
 *
 * =====================================================================================
 */

#ifndef myApp_h
#define myApp_h

#include "engine/cw_header.hpp"
#include "engine/gl_header.hpp"

#include "engine/cw_app.hpp"

#include "engine/cw_camera.hpp"

#include "engine/LoadJSON.hpp"

#include <vector>

namespace cw
{
    class Shader;
    struct Mesh;
	struct Texture;
    
    class Sound;
    /*-----------------------------------------------------------------------------
     *  OUR APP
     *-----------------------------------------------------------------------------*/
    class MyApp final : public App{
        
    private:
        
        Shader *shader;

		std::vector<Mesh*> meshes;
		std::vector<Texture*> textures;
		std::vector<glm::mat3> normalMatrixes;

		LoadJSON json;

        Mesh *mesh;
        Mesh *mesh2;
		Mesh *mesh3;
        
        Texture *texture;
        Texture *texture2;
		Texture *texture3;
        
        cw::Camera *m_camera;
        
        //ID of Vertex Attribute
        GLuint positionID, normalID, colorID, textureCoorID;
        
        //A buffer ID
        GLuint bufferID, elementID;
        
        //An array ID
        GLuint arrayID;
        
        //ID of Uniforms
        GLuint modelID, viewID, projectionID, normalMatID;
        
        
        // keep input status
        bool isMouseDown;
        int previousX;
        int previousY;
        
        Sound* music;
        
    public:
        MyApp() { init(); }
        ~MyApp();
        MyApp( const MyApp& ) = default;
        
        /*-----------------------------------------------------------------------------
         *  Overrided Functions
         *-----------------------------------------------------------------------------*/
        virtual void init();
        virtual void onUpdate( float dtSecond ) override;
        virtual void onDraw() override;
        virtual void onMouseMoveEvent(int x, int y) override;
        virtual void onMouseEvent(int button, int action) override;
        virtual void onKeyEvent(int key, int action) override;
        
        
    };
    
}
#endif /* myApp_h */
