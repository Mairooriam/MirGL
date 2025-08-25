#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Mir {
    enum class CameraMovement {
        FORWARD,
        BACKWARD,
        UP,    // For ortho
        DOWN,  // For ortho
        LEFT,
        RIGHT,
        ROTATE_RIGHT,
        ROTATE_LEFT,
        ROTATE_UP,
        ROTATE_DOWN
    };

    // Default camera values
    const float YAW = -90.0f;
    const float PITCH = 0.0f;
    const float SPEED = 2.5f;
    const float SENSITIVITY = 1.0f;
    const float ZOOM = 45.0f;

    class Camera {
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
        Camera(
            glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
            float yaw = YAW, float pitch = PITCH);

        Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

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
            return {m_Position, m_Front, m_Up,   m_Right,         m_WorldUp,
                    m_Yaw,      m_Pitch, m_Zoom, m_MovementSpeed, m_MouseSensitivity};
        }

      private:
        void updateCameraVectors();
    };

    class OrthoCamera {
      private:
        glm::vec3 m_Position;  // Camera position
        float m_Zoom;          // Zoom level
        float m_WindowWidth;   // Window width
        float m_WindowHeight;  // Window height
        float m_Near;          // Near clipping plane
        float m_Far;           // Far clipping plane

      public:
        OrthoCamera(
            float windowWidth, float windowHeight, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), float zoom = 1.0f,
            float near = -1.0f, float far = 1.0f)
            : m_Position(position),
              m_Zoom(zoom),
              m_WindowWidth(windowWidth),
              m_WindowHeight(windowHeight),
              m_Near(near),
              m_Far(far) {}

        glm::mat4 GetViewMatrix() const { return glm::translate(glm::mat4(1.0f), -m_Position); }

        glm::mat4 GetProjectionMatrix() const {
            float halfWidth = m_WindowWidth / (2.0f * m_Zoom);
            float halfHeight = m_WindowHeight / (2.0f * m_Zoom);
            return glm::ortho(
                -halfWidth, halfWidth,    // Left, Right
                -halfHeight, halfHeight,  // Bottom, Top
                m_Near, m_Far             // Near, Far
            );
        }

        void ProcessKeyboard(CameraMovement direction, float deltaTime) {
            float velocity = 500.0f * deltaTime;                               // Adjust speed as needed
            if (direction == CameraMovement::UP) m_Position.y -= velocity;     // Move up
            if (direction == CameraMovement::DOWN) m_Position.y += velocity;   // Move down
            if (direction == CameraMovement::LEFT) m_Position.x -= velocity;   // Move left
            if (direction == CameraMovement::RIGHT) m_Position.x += velocity;  // Move right
        }

        void ProcessMouseScroll(float yoffset) {
            m_Zoom -= yoffset * 0.1f;            // Adjust zoom sensitivity
            if (m_Zoom < 0.1f) m_Zoom = 0.1f;    // Minimum zoom level
            if (m_Zoom > 10.0f) m_Zoom = 10.0f;  // Maximum zoom level
        }

        glm::vec3 GetPosition() const { return m_Position; }
        float GetZoom() const { return m_Zoom; }
        float GetNear() const { return m_Near; }
        float GetFar() const { return m_Far; }
        void SetNear(float near_) { m_Near = near_; }
        void SetFar(float far_) { m_Far = far_; }
    };

}  // namespace Mir