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
    const float SPEED = 5.0f;
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
        float GetYaw() const { return m_Yaw; }
        float GetPitch() const { return m_Pitch; }
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
        glm::vec3 ScreenToWorld(float x_screen, float y_screen, float windowWidth, float windowHeight) const {
            // Normalize screen coordinates to [-1, 1]
            float x_ndc = (2.0f * x_screen) / windowWidth - 1.0f;
            float y_ndc = 1.0f - (2.0f * y_screen) / windowHeight;  // Flip Y because screen Y is top-down

            // Convert NDC to world space
            float halfWidth = m_WindowWidth * (m_Zoom / 2.0f);
            float halfHeight = m_WindowHeight * (m_Zoom / 2.0f);

            float worldX = x_ndc * halfWidth + m_Position.x;
            float worldY = y_ndc * halfHeight + m_Position.y;

            return glm::vec3(worldX, worldY, 0.0f);
        }

        // glm::vec3 ScreenToWorld(float x_screen, float y_screen, float windowWidth, float windowHeight) const {
        //     // Step 1: Normalize screen coordinates to NDC [-1, 1]
        //     float x_ndc = (2.0f * x_screen) / windowWidth - 1.0f;
        //     float y_ndc = 1.0f - (2.0f * y_screen) / windowHeight;  // Flip Y because screen Y is top-down
        //     glm::vec4 ndcPos(x_ndc, y_ndc, 0.0f, 1.0f);             // Z = 0 for 2D

        //     // Step 2: Get the inverse projection and view matrices
        //     glm::mat4 inverseProjection = glm::inverse(GetProjectionMatrix());
        //     glm::mat4 inverseView = glm::inverse(GetViewMatrix());

        //     // Step 3: Transform NDC to world space
        //     glm::vec4 worldPos = inverseProjection * inverseView * ndcPos;

        //     // Step 4: Return the world position (divide by w to handle homogeneous coordinates)
        //     return glm::vec3(worldPos) / worldPos.w;
        // }
        glm::mat4 GetProjectionMatrix() const {
            float halfWidth = m_WindowWidth * (m_Zoom / 2.0f);
            float halfHeight = m_WindowHeight * (m_Zoom / 2.0f);
            return glm::ortho(
                -halfWidth, halfWidth,    // Left, Right
                -halfHeight, halfHeight,  // Bottom, Top
                m_Near, m_Far             // Near, Far
            );
        }

        void ProcessKeyboard(CameraMovement direction, float deltaTime) {
            float velocity = 20.0f * deltaTime;                                // Adjust speed as needed
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