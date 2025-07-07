#include "Camera.h"

namespace Mir
{
    Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) 
        : m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), 
          m_MovementSpeed(SPEED), 
          m_MouseSensitivity(SENSITIVITY), 
          m_Zoom(ZOOM)
    {
        m_Position = position;
        m_WorldUp = up;
        m_Yaw = yaw;
        m_Pitch = pitch;
        updateCameraVectors();
    }

    Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) 
        : m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), 
          m_MovementSpeed(SPEED), 
          m_MouseSensitivity(SENSITIVITY), 
          m_Zoom(ZOOM)
    {
        m_Position = glm::vec3(posX, posY, posZ);
        m_WorldUp = glm::vec3(upX, upY, upZ);
        m_Yaw = yaw;
        m_Pitch = pitch;
        updateCameraVectors();
    }

    glm::mat4 Camera::GetViewMatrix()
    {
        return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
    }

    void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
    {
        float velocity = m_MovementSpeed * deltaTime;
        bool needsUpdate = false;
        
        if (direction == CameraMovement::FORWARD)
            m_Position += m_Front * velocity;
        if (direction == CameraMovement::BACKWARD)
            m_Position -= m_Front * velocity;
        if (direction == CameraMovement::LEFT)
            m_Position -= m_Right * velocity;
        if (direction == CameraMovement::RIGHT)
            m_Position += m_Right * velocity;
        if (direction == CameraMovement::ROTATE_RIGHT) {
            m_Yaw += m_MouseSensitivity * 50.0f * deltaTime; 
            needsUpdate = true;
        }
        if (direction == CameraMovement::ROTATE_LEFT) {
            m_Yaw -= m_MouseSensitivity * 50.0f * deltaTime;
            needsUpdate = true;
        }
        if (direction == CameraMovement::ROTATE_UP) {
            m_Pitch += m_MouseSensitivity * 50.0f * deltaTime;
            needsUpdate = true;
        }
        if (direction == CameraMovement::ROTATE_DOWN) {
            m_Pitch -= m_MouseSensitivity * 50.0f * deltaTime;
            needsUpdate = true;
        }
        
        if (needsUpdate) {
            if (m_Pitch > 89.0f)
                m_Pitch = 89.0f;
            if (m_Pitch < -89.0f)
                m_Pitch = -89.0f;
            
            updateCameraVectors(); 
        }

    }

    void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
    {
        xoffset *= m_MouseSensitivity;
        yoffset *= m_MouseSensitivity;

        m_Yaw += xoffset;
        m_Pitch += yoffset;

        if (constrainPitch)
        {
            if (m_Pitch > 89.0f)
                m_Pitch = 89.0f;
            if (m_Pitch < -89.0f)
                m_Pitch = -89.0f;
        }

        updateCameraVectors();
    }

    void Camera::ProcessMouseScroll(float yoffset)
    {
        m_Zoom -= (float)yoffset;
        if (m_Zoom < 1.0f)
            m_Zoom = 1.0f;
        if (m_Zoom > 45.0f)
            m_Zoom = 45.0f;
    }

    void Camera::updateCameraVectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        front.y = sin(glm::radians(m_Pitch));
        front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
        m_Front = glm::normalize(front);
        
        m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));  
        m_Up = glm::normalize(glm::cross(m_Right, m_Front));
    }

} // namespace Mir