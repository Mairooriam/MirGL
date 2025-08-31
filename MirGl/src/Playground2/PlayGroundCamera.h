#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <variant>

namespace Mir {
    namespace Playground2 {
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
          public:
            virtual ~Camera() = default;

            // Common interface for all cameras
            virtual glm::mat4 GetViewMatrix() const = 0;
            virtual glm::mat4 GetProjectionMatrix() const = 0;
            virtual glm::vec3 GetPosition() const = 0;
            virtual void ProcessKeyboard(CameraMovement direction, float deltaTime) = 0;
            virtual void ProcessMouseScroll(float yoffset) = 0;
            virtual glm::vec3 ScreenToWorld(
                float x_screen, float y_screen, float windowWidth, float windowHeight) const = 0;
        };

        class PerspectiveCamera : public Camera {
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
            float m_AspectRatio = 1.0f;  // Default aspect ratio

          public:
            PerspectiveCamera(
                glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
                float yaw = YAW, float pitch = PITCH);

            glm::mat4 GetViewMatrix() const override;
            glm::mat4 GetProjectionMatrix() const override;
            glm::vec3 GetPosition() const override;
            glm::vec3 ScreenToWorld(float x_screen, float y_screen, float windowWidth, float windowHeight) const override;

            void ProcessKeyboard(CameraMovement direction, float deltaTime) override;
            void ProcessMouseScroll(float yoffset) override;

            void SetAspectRatio(float aspectRatio) { m_AspectRatio = aspectRatio; }

          private:
            void updateCameraVectors();
        };

        class OrthoCamera : public Camera {
          private:
            glm::vec3 m_Position;  // Camera position
            float m_Zoom;          // Zoom level
            float m_WindowWidth;   // Window width
            float m_WindowHeight;  // Window height
            float m_Near;          // Near clipping plane
            float m_Far;           // Far clipping plane

          public:
            OrthoCamera(
                float windowWidth, float windowHeight, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
                float zoom = 1.0f, float near = -1.0f, float far = 1.0f);

            glm::mat4 GetViewMatrix() const override;
            glm::mat4 GetProjectionMatrix() const override;
            glm::vec3 GetPosition() const override;
            glm::vec3 ScreenToWorld(float x_screen, float y_screen, float windowWidth, float windowHeight) const override;

            void ProcessKeyboard(CameraMovement direction, float deltaTime) override;
            
            void ProcessMouseScroll(float yoffset) override;
        };

    }  // namespace Playground2
}  // namespace Mir