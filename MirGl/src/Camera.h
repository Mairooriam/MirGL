#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Mir
{
    enum class CameraMovement {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        ROTATE_RIGHT,
        ROTATE_LEFT,
        ROTATE_UP,
        ROTATE_DOWN
    };

    // Default camera values
    const float YAW         = -90.0f;
    const float PITCH       =  0.0f;
    const float SPEED       =  2.5f;
    const float SENSITIVITY =  1.0f;
    const float ZOOM        =  45.0f;

    class Camera
    {
    private:
        glm::vec3 m_Position;
        glm::vec3 m_Front;
        glm::vec3 m_Up;
        glm::vec3 m_Right;
        glm::vec3 m_WorldUp;

        float m_Yaw;
        float m_Pitch;

        float m_MovementSpeed;
        float m_MouseSensitivity;
        float m_Zoom;

    public:
        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
               glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
               float yaw = YAW, 
               float pitch = PITCH);

        Camera(float posX, float posY, float posZ, 
               float upX, float upY, float upZ, 
               float yaw, float pitch);

        glm::mat4 GetViewMatrix();
        void ProcessKeyboard(CameraMovement direction, float deltaTime);
        void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
        void ProcessMouseScroll(float yoffset);

        glm::vec3 GetPosition() const { return m_Position; }
        glm::vec3 GetFront() const { return m_Front; }
        glm::vec3 GetUp() const { return m_Up; }
        glm::vec3 GetRight() const { return m_Right; }
        float GetZoom() const { return m_Zoom; }

    struct CameraState {
        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 worldUp;
        float yaw;
        float pitch;
        float zoom;
        float movementSpeed;
        float mouseSensitivity;
    };

    CameraState GetCameraState() const {
        return {
            m_Position,
            m_Front,
            m_Up,
            m_Right,
            m_WorldUp,
            m_Yaw,
            m_Pitch,
            m_Zoom,
            m_MovementSpeed,
            m_MouseSensitivity
        };
    }

    private:
        void updateCameraVectors();
    };
    
} // namespace Mir