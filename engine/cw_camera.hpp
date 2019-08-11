/*
 * =====================================================================================
 *
 *         A simple game engine for ITGT533
 *
 *         Author:  Chatchai Wangwiwattana
 *
 * =====================================================================================
 */
#ifndef cw_camera_hpp
#define cw_camera_hpp

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace cw
{
    
    struct Camera
    {

        glm::vec3 m_position;

        float m_near;
        float m_far;
        float m_fov;
        
        float m_yaw;
        float m_pitch;
        float m_roll;

		glm::quat rotation;

        Camera()
        {
            m_position = glm::vec3(0, 0, 0);
            
            m_near = 0.1f;
            m_far = -10.0f;
            m_fov = 3.14f/3.f;
            
            m_yaw = 0.0f;
            m_pitch = 0.0f;
            m_roll = 0.0f;
        }
        
        
        inline glm::mat4 getViewMat()
        {

            glm::mat4 invertPosition = glm::translate(glm::mat4(1),
                                                      glm::vec3(-m_position.x,
                                                                -m_position.y,
                                                                -m_position.z));
            
            return getRotationMat() * invertPosition;
        }
        
        
        inline glm::mat4 getProjectionMat( int window_width, int window_height ) const
        {
            return glm::perspective( m_fov, static_cast<float>(window_width)/window_height, m_near, m_far);
        }
        
        inline glm::mat4 getRotationMat()
        {
			rotation = glm::quat(glm::vec3(m_pitch, m_yaw, m_roll));
			return glm::toMat4(rotation);
        }
        
        
        inline glm::vec3 getPosition() const
        {
            return m_position;
        }
        
        
        inline void setPosition( const glm::vec3& _position )
        {
            m_position = _position;
        }
        
        
    };
}

#endif /* cw_camera_hpp */
