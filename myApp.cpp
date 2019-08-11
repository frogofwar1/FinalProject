/*
 * =====================================================================================
 *
 *         A simple game engine for ITGT533
 *
 *         Author:  Chatchai Wangwiwattana
 *
 * =====================================================================================
 */


#include "myApp.hpp"
#include "engine/gl_header.hpp"


#include <iostream>
#include <string>


#include "engine/cw_app.hpp"
#include "engine/gl_shader.hpp"
#include "engine/gl_macros.hpp"
#include "engine/gl_mesh.hpp"
#include "engine/gl_texture.hpp"

#include "engine/cw_camera.hpp"
#include "engine/gl_data.hpp"

#include "engine/cw_sound.hpp"

#include "engine/LoadOBJ.hpp"

using glm::vec2;
using glm::vec3;
using glm::vec4;

namespace cw
{
    
    
    using namespace std;
    
    /*-----------------------------------------------------------------------------
     *  SHADER CODE
     *-----------------------------------------------------------------------------*/
    const char * vert = GLSL(120,
                             
                             attribute vec3 position;
                             attribute vec3 normal;
                             attribute vec4 color;
                             attribute vec2 textureCoordinate;
                             
                             uniform mat4 projection;
                             uniform mat4 view;
                             uniform mat4 model;
                             uniform mat3 normalMatrix;
                             
                             varying vec2 texCoord;
                             varying float diffuse;
                             varying vec4 thecolor;
                             
                             //the simplest function to calculate lighting
                             float doColor(){
                                 vec3 norm  = normalize( normalMatrix * normalize(normal) );
                                 vec3 light = normalize( vec3(1.0, 1.0, 1.0) );
                                 diffuse = max(dot(norm, light), 0.0 );
                                 
                                 return diffuse;
                             }
                             
                             void main(void){
                                 thecolor = color;
                                 diffuse = doColor();
                                 texCoord = textureCoordinate;
                                 gl_Position = projection * view * model * vec4(position, 1);
                             }
                             
                             );
    
    
    
    const char * frag = GLSL(120,
                             
                             uniform sampler2D sampler;
                             
                             varying vec2 texCoord;
                             varying float diffuse;
                             varying vec4 thecolor;
                             
                             void main(void){
                                 gl_FragColor =  vec4( texture2D( sampler, texCoord ).rgb * thecolor.rgb * diffuse, 1.0);
                             }
                             
                             );
    

    
    void MyApp::init()
    {
		json.loadJsonFile();
        
        //Specify the 8 VERTICES of A Cube
        //                  position          normal          color          texturecoord
        Vertex cube[] = {
            { vec3( 1, -1,  1), vec3( 1,-1, 1), vec4(.5,.5,.5,1), vec2(1,0) } ,
            { vec3( 1,  1,  1), vec3( 1, 1, 1), vec4(.5,.5,.5,1), vec2(1,1) } ,
            { vec3(-1,  1,  1), vec3(-1, 1, 1), vec4(.5,.5,.5,1), vec2(0,1) } ,
            { vec3(-1, -1,  1), vec3(-1,-1, 1), vec4(.5,.5,.5,1), vec2(0,0) } ,
            
            { vec3( 1, -1, -1), vec3( 1,-1,-1), vec4(.5,.5,.5,1), vec2(0,0) } ,
            { vec3( 1,  1, -1), vec3( 1, 1,-1), vec4(.5,.5,.5,1), vec2(0,1) } ,
            { vec3(-1,  1, -1), vec3(-1, 1,-1), vec4(.5,.5,.5,1), vec2(1,1) } ,
            { vec3(-1, -1, -1), vec3(-1,-1,-1), vec4(.5,.5,.5,1), vec2(1,0) }
        };
        
        
           //6--------------/5
          //  .           // |
        //2--------------1   |
        //    .          |   |
        //    .          |   |
        //    .          |   |
        //    .          |   |
        //    7.......   |   /4
        //               | //
        //3--------------/0
        
        GLubyte indices[24] =
        {
            0,1,2,3, //front
            7,6,5,4, //back
            3,2,6,7, //left
            4,5,1,0, //right
            1,5,6,2, //top
            4,0,3,7 }; //bottom
        
        // create shader program
        shader = new Shader( vert, frag );

        // create a mesh
        mesh = new Mesh();
        mesh->vertices.assign( std::begin(cube), std::end(cube) );
        mesh->indices.assign( std::begin(indices), std::end(indices) );
        mesh->getAttributes(shader->id());
        mesh->buffer();
        mesh->mScale = 0.3f;
        mesh->mPos = vec3(1,1,0);
        
        // create a second mesh
        mesh2 = new Mesh();
        mesh2->vertices.assign( std::begin(cube), std::end(cube) );
        mesh2->indices.assign( std::begin(indices), std::end(indices) );
        mesh2->getAttributes(shader->id());
        mesh2->buffer();
        mesh2->mScale = 0.4f;
        mesh2->mPos = vec3(-1,-1,0);

		// create a third mesh
		LoadOBJ* obj1 = new LoadOBJ("resources/house.obj");
		mesh3 = new Mesh();
		mesh3->vertices.assign(std::begin(obj1->vertices), std::end(obj1->vertices));
		mesh3->indices.assign(std::begin(obj1->indices), std::end(obj1->indices));
		mesh3->getAttributes(shader->id());
		mesh3->buffer();
		mesh3->mScale = 5.0f;
		mesh3->mPos = vec3(0, -5, -5);
		delete(obj1);
        
        // create an image1 and transfer it to GPU with Texture class
        Image* image1 = new Image("resources/box.png");
        texture = new Texture(image1->width, image1->height, image1->channel);
        texture->bind();
        texture->update( image1->data );
        texture->unbind();
        delete(image1);
        
        // create an image2 and transfer it to GPU with Texture class
        Image* image2 = new Image("resources/brick.jpg");
        texture2 = new Texture(image2->width, image2->height, image2->channel);
        texture2->bind();
        texture2->update( image2->data );
        texture2->unbind();
        delete(image2);
        
		// create an image3 and transfer it to GPU with Texture class
		Image* image3 = new Image("resources/house.png");
		texture3 = new Texture(image3->width, image3->height, image3->channel);
		texture3->bind();
		texture3->update(image3->data);
		texture3->unbind();
		delete(image3);
        
        // Get uniform locations
        modelID = glGetUniformLocation(shader->id(), "model");
        viewID = glGetUniformLocation(shader->id(), "view");
        projectionID = glGetUniformLocation(shader-> id(), "projection");
        normalMatID = glGetUniformLocation(shader->id(), "normalMatrix");
        

        // Setup Camera
        m_camera = new Camera();
        m_camera->setPosition(glm::vec3(0, 0, 5));
        
        
        // Init input
        isMouseDown = false;
        
        
        // Setup Sound
        //soundEffect1 = new Sound("resources/maybe-next-time-huh.wav");
        //soundEffect1->play();

        //Sound * bgSound = new Sound("resources/airplane-interior-1.mp3");
        //bgSound->play();

    }
    
    
    void MyApp::onUpdate( float dtSecond )
    {
        
        /*-----------------------------------------------------------------------------
         *  Update camera's position
         *-----------------------------------------------------------------------------*/
        const float hSpeed = 0.1f;
        const float vSpeed = 0.1f;
		const float uSpeed = 0.1f;
        
        if( isKeyDown(GLFW_KEY_W) )
        {
            glm::vec4 v =  glm::inverse(m_camera->getRotationMat()) * glm::vec4(0,0,hSpeed,0);
            m_camera->setPosition( m_camera->getPosition() - glm::vec3(v) );
        }
        if( isKeyDown(GLFW_KEY_S) )
        {
            glm::vec4 v = glm::inverse(m_camera->getRotationMat()) * glm::vec4(0,0,hSpeed,0);
            m_camera->setPosition( m_camera->getPosition() + glm::vec3(v) );
        }
        if( isKeyDown(GLFW_KEY_A) )
        {
            glm::vec4 v = glm::inverse(m_camera->getRotationMat()) * glm::vec4(vSpeed,0,0,0);
            m_camera->setPosition( m_camera->getPosition() - glm::vec3(v) );

        }
        if( isKeyDown(GLFW_KEY_D) )
        {
            glm::vec4 v = glm::inverse(m_camera->getRotationMat()) * glm::vec4(vSpeed,0,0,0);
            m_camera->setPosition( m_camera->getPosition() + glm::vec3(v) );

        }
		if (isKeyDown(GLFW_KEY_LEFT_SHIFT))
		{
			glm::vec4 v = glm::inverse(m_camera->getRotationMat()) * glm::vec4(0, uSpeed, 0, 0);
			m_camera->setPosition(m_camera->getPosition() + glm::vec3(v));

		}
		if (isKeyDown(GLFW_KEY_LEFT_CONTROL))
		{
			glm::vec4 v = glm::inverse(m_camera->getRotationMat()) * glm::vec4(0, uSpeed, 0, 0);
			m_camera->setPosition(m_camera->getPosition() - glm::vec3(v));

		}
        
    }
    
    
    void MyApp::onDraw()
    {
        /*-----------------------------------------------------------------------------
         *  Draw GUI
         *-----------------------------------------------------------------------------*/
		static bool loadMusicFile = false;
		ImGui::Begin("Controller"); // Create a windowl
        
        ImGui::Text("Music Control."); // Display some text
        
		static const char * current_music = json.musics[0].c_str();
		if (ImGui::BeginCombo("Select music", current_music)) // The second parameter is the label previewed before opening the combo.
		{
			for (int n = 0; n < json.musics.size(); n++)
			{
				bool is_selected = (current_music == json.musics[n].c_str()); // You can store your selection however you want, outside or inside your objects
				if (ImGui::Selectable(json.musics[n].c_str(), is_selected))
					current_music = json.musics[n].c_str();
				if (is_selected)
					ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
			}
			ImGui::EndCombo();
		}
		static bool isPlay = false;
		if (!isPlay) {
			ImGui::SameLine();
			if (ImGui::Button("Load Music")) {
				music = new Sound(current_music);
				loadMusicFile = true;
			}
		}
		if (loadMusicFile) {
			if (!isPlay) {
				if (ImGui::Button("Play")) {
					music->play();
					isPlay = true;
				}
			}
			if (isPlay) {
				if (ImGui::Button("Pause")) {
					music->pause();
					isPlay = false;
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Stop")) {
				music->stop();
				isPlay = false;
				loadMusicFile = false;
			}
		}

		ImGui::NewLine();
		ImGui::Text("Load Object Control.");
		static float spos[3];
		ImGui::InputFloat3("Position to spawn", spos);

		static int OBJNo = 4;
		static const char * current_OBJ = json.models[OBJNo].model.c_str();
		if (ImGui::BeginCombo("Select OBJ", current_OBJ)) // The second parameter is the label previewed before opening the combo.
		{
			for (int n = 0; n < json.models.size(); n++)
			{
				bool is_selected = (current_OBJ == json.models[n].model.c_str()); // You can store your selection however you want, outside or inside your objects
				if (ImGui::Selectable(json.models[n].model.c_str(), is_selected))
				{
					current_OBJ = json.models[n].model.c_str();
					OBJNo = n;
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
			}
			ImGui::EndCombo();
		}
		ImGui::SameLine();
		if (ImGui::Button("Load OBJ"))
		{
			LoadOBJ* obj = new LoadOBJ(current_OBJ);
			meshes.push_back(new Mesh());
			meshes[meshes.size() - 1]->vertices.assign(std::begin(obj->vertices), std::end(obj->vertices));
			meshes[meshes.size() - 1]->indices.assign(std::begin(obj->indices), std::end(obj->indices));
			meshes[meshes.size() - 1]->getAttributes(shader->id());
			meshes[meshes.size() - 1]->buffer();
			meshes[meshes.size() - 1]->mPos = vec3(spos[0], spos[1], spos[2]);
			delete(obj);

			if (json.models[OBJNo].texture != "") {
				Image* image = new Image(json.models[OBJNo].texture.c_str());
				textures.push_back(new Texture(image->width, image->height, image->channel));
				textures[textures.size() - 1]->bind();
				textures[textures.size() - 1]->update(image->data);
				textures[textures.size() - 1]->unbind();
				delete(image);
			}
			else {
				textures.push_back(new Texture(0, 0, 0));
			}
		}

		ImGui::NewLine();
		ImGui::Text("Select Object Control.");
		static int objectNo;
		static const char * current_Object = "None Selected";
		static bool isSelectObject = false;
		if (ImGui::BeginCombo("Select Object", current_Object)) // The second parameter is the label previewed before opening the combo.
		{
			for (int n = 0; n < meshes.size(); n++)
			{
				bool is_selected = (current_Object == to_string(n).c_str()); // You can store your selection however you want, outside or inside your objects
				if (ImGui::Selectable(to_string(n).c_str(), is_selected))
				{
					current_Object = to_string(n).c_str();
					objectNo = n;
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
				isSelectObject = true;
			}
			ImGui::EndCombo();
		}
		if (isSelectObject) {
			static float tpos[3] = { meshes[objectNo]->mPos.x, meshes[objectNo]->mPos.y, meshes[objectNo]->mPos.z };
			tpos[0] = meshes[objectNo]->mPos.x;
			tpos[1] = meshes[objectNo]->mPos.y;
			tpos[2] = meshes[objectNo]->mPos.z;
			ImGui::InputFloat3("Position", tpos);
			static int trpos[3] = {0, 1, 0};
			ImGui::SliderInt3("Axis", trpos, 0, 1);
			static float radian = 0.0f;
			ImGui::SliderFloat("Rotation", &radian, 0.0f, 2.0f * PI);

			meshes[objectNo]->mPos = vec3(tpos[0], tpos[1], tpos[2]);
			meshes[objectNo]->mRot = glm::rotate(glm::mat4(1.0f), radian, glm::vec3(static_cast<float>(trpos[0]), static_cast<float>(trpos[1]), static_cast<float>(trpos[2])));
		}
        
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
        
        
        /*-----------------------------------------------------------------------------
         *  Draw Game World
         *-----------------------------------------------------------------------------*/
        
        // enable shader program
        shader->bind();
       
        // prepare matrix transformation
        glm::mat4 view = m_camera->getViewMat();
        glm::mat4 proj = m_camera->getProjectionMat(window().width(), window().height());
        glUniformMatrix4fv( viewID, 1, GL_FALSE, glm::value_ptr(view) );
        glUniformMatrix4fv( projectionID, 1, GL_FALSE, glm::value_ptr(proj) );
        
        
        /*-----------------------------------------------------------------------------
         *  Draw first cube
         *-----------------------------------------------------------------------------*/
        
        static float angle = 0.0;
        angle += 0.01f;
        
        
        mesh->mRot = glm::rotate( glm::quat(), angle, glm::vec3(0,1,0));
        glUniformMatrix4fv( modelID, 1, GL_FALSE, glm::value_ptr(mesh->model()) );

        // pass normal matrix to calculate lighting
        glm::mat3 normalMatrix = glm::transpose( glm::inverse( glm::mat3(view * mesh->model()) ) );
        glUniformMatrix3fv( normalMatID, 1, GL_FALSE, glm::value_ptr(normalMatrix) );
        // use texture1 to draw one cube
        texture->bind();
            mesh->bind();
            mesh->drawElements(GL_QUADS);
            mesh->unbind();
        texture->unbind();
        
        /*-----------------------------------------------------------------------------
         *  Draw second cube
         *-----------------------------------------------------------------------------*/
        glm::mat3 normalMatrix2 = glm::transpose( glm::inverse( glm::mat3(view * mesh2->model()) ) );
        glUniformMatrix3fv( normalMatID, 1, GL_FALSE, glm::value_ptr(normalMatrix2) );
        mesh2->mRot = glm::rotate( glm::quat(), angle, glm::vec3(0,1,0));
        glUniformMatrix4fv( modelID, 1, GL_FALSE, glm::value_ptr(mesh2->model()) );
        
        texture2->bind();
            mesh2->bind();
            mesh2->drawElements(GL_QUADS);
            mesh2->unbind();
        texture2->unbind();

		/*-----------------------------------------------------------------------------
		 *  Draw cat
		 *-----------------------------------------------------------------------------*/
		glm::mat3 normalMatrix3 = glm::transpose(glm::inverse(glm::mat3(view * mesh3->model())));
		glUniformMatrix3fv(normalMatID, 1, GL_FALSE, glm::value_ptr(normalMatrix3));
		mesh3->mRot = glm::rotate(glm::quat(), angle, glm::vec3(0, 1, 0));
		glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(mesh3->model()));

		texture3->bind();
			mesh3->bind();
			mesh3->drawArrays(GL_TRIANGLES);
			mesh3->unbind();
		texture3->unbind();

		for (int i = 0; i < meshes.size(); i++) {
			normalMatrixes.push_back(glm::transpose(glm::inverse(glm::mat3(view * meshes[i]->model()))));
			glUniformMatrix3fv(normalMatID, 1, GL_FALSE, glm::value_ptr(normalMatrixes[i]));
			//meshes[i]->mRot = glm::rotate(glm::quat(), angle, glm::vec3(0, 1, 0));
			glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(meshes[i]->model()));
		
			textures[i]->bind();
				meshes[i]->bind();
				meshes[i]->drawArrays(GL_TRIANGLES);
				meshes[i]->unbind();
			textures[i]->unbind();
		}

        //unsued shader program
        shader->unbind();
        
    }

    
    void MyApp::onKeyEvent(int key, int action)
    {
        /*-----------------------------------------------------------------------------
         *  exit the game when press esc
         *-----------------------------------------------------------------------------*/
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            exit(0);
        }
        
    }
    
    void MyApp::onMouseEvent(int button, int action)
    {
        /*-----------------------------------------------------------------------------
         *  Update mouse state to move camera
         *-----------------------------------------------------------------------------*/
        if(button == GLFW_MOUSE_BUTTON_1 && action == GLFW_PRESS )
        {
            isMouseDown = true;
        }
        if(button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE )
        {
            isMouseDown = false;
        }
    }
    
    void MyApp::onMouseMoveEvent(int x, int y)
    {
        
        /*-----------------------------------------------------------------------------
         *  Update mouse state to move camera
         *-----------------------------------------------------------------------------*/
        App::onMouseMoveEvent(x, y);
        
        if( isMouseDown )
        {
            const float DRAG = 0.01f;
            
            int xoffset = x - previousX;
            int yoffset = y - previousY;
            m_camera->m_yaw += xoffset*DRAG;
            m_camera->m_pitch += yoffset*DRAG;
        }
        
        previousX = x;
        previousY = y;
    }
    
    
    MyApp::~MyApp()
    {
        // clean up everything
        delete(texture);
        delete(texture2);
        delete(mesh);
        delete(mesh2);
        delete(shader);
        delete(m_camera);
		for (int i = 0; i < meshes.size(); i++)
			delete(meshes[i]);
		for (int i = 0; i < textures.size(); i++)
			delete(textures[i]);
    }
    
}
