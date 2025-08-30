#include "PlayGroundCamera.h"

namespace Mir {
    namespace Playground2 {

        // PerspectiveCamera Implementation
        PerspectiveCamera::PerspectiveCamera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
            : m_Position(position), m_WorldUp(up), m_Yaw(yaw), m_Pitch(pitch),
              m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), m_MovementSpeed(SPEED), m_MouseSensitivity(SENSITIVITY),
              m_Zoom(ZOOM) {
            updateCameraVectors();
        }

        glm::mat4 PerspectiveCamera::GetViewMatrix() const {
            return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
        }

        glm::mat4 PerspectiveCamera::GetProjectionMatrix() const {
            return glm::perspective(glm::radians(m_Zoom), m_AspectRatio, 0.1f, 100.0f);
        }

        glm::vec3 PerspectiveCamera::GetPosition() const {
            return m_Position;
        }

        glm::vec3 PerspectiveCamera::ScreenToWorld(float x_screen, float y_screen, float windowWidth, float windowHeight) const {
            float x_ndc = (2.0f * x_screen) / windowWidth - 1.0f;
            float y_ndc = 1.0f - (2.0f * y_screen) / windowHeight;

            glm::vec4 clipSpacePos(x_ndc, y_ndc, -1.0f, 1.0f);
            glm::mat4 inverseVP = glm::inverse(GetProjectionMatrix() * GetViewMatrix());
            glm::vec4 worldSpacePos = inverseVP * clipSpacePos;

            if (worldSpacePos.w != 0.0f) {
                worldSpacePos /= worldSpacePos.w;
            }

            return glm::vec3(worldSpacePos);
        }

        void PerspectiveCamera::ProcessKeyboard(CameraMovement direction, float deltaTime) {
            float velocity = m_MovementSpeed * deltaTime;
            if (direction == CameraMovement::FORWARD) m_Position += m_Front * velocity;
            if (direction == CameraMovement::BACKWARD) m_Position -= m_Front * velocity;
            if (direction == CameraMovement::LEFT) m_Position -= m_Right * velocity;
            if (direction == CameraMovement::RIGHT) m_Position += m_Right * velocity;
        }

        void PerspectiveCamera::ProcessMouseScroll(float yoffset) {
            m_Zoom -= yoffset;
            if (m_Zoom < 1.0f) m_Zoom = 1.0f;
            if (m_Zoom > 45.0f) m_Zoom = 45.0f;
        }

        void PerspectiveCamera::updateCameraVectors() {
            glm::vec3 front;
            front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
            front.y = sin(glm::radians(m_Pitch));
            front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
            m_Front = glm::normalize(front);
            m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
            m_Up = glm::normalize(glm::cross(m_Right, m_Front));
        }

        // OrthoCamera Implementation
        OrthoCamera::OrthoCamera(float windowWidth, float windowHeight, glm::vec3 position, float zoom, float near, float far)
            : m_WindowWidth(windowWidth), m_WindowHeight(windowHeight), m_Position(position), m_Zoom(zoom), m_Near(near), m_Far(far) {}

        glm::mat4 OrthoCamera::GetViewMatrix() const {
            return glm::translate(glm::mat4(1.0f), -m_Position);
        }

        glm::mat4 OrthoCamera::GetProjectionMatrix() const {
            float halfWidth = m_WindowWidth * (m_Zoom / 2.0f);
            float halfHeight = m_WindowHeight * (m_Zoom / 2.0f);
            return glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, m_Near, m_Far);
        }

        glm::vec3 OrthoCamera::GetPosition() const {
            return m_Position;
        }

        glm::vec3 OrthoCamera::ScreenToWorld(float x_screen, float y_screen, float windowWidth, float windowHeight) const {
            float x_ndc = (2.0f * x_screen) / windowWidth - 1.0f;
            float y_ndc = 1.0f - (2.0f * y_screen) / windowHeight;

            float halfWidth = m_WindowWidth * (m_Zoom / 2.0f);
            float halfHeight = m_WindowHeight * (m_Zoom / 2.0f);

            float worldX = x_ndc * halfWidth + m_Position.x;
            float worldY = y_ndc * halfHeight + m_Position.y;

            return glm::vec3(worldX, worldY, 0.0f);
        }

        void OrthoCamera::ProcessKeyboard(CameraMovement direction, float deltaTime) {
            float velocity = 20.0f * deltaTime;
            if (direction == CameraMovement::UP) m_Position.y -= velocity;
            if (direction == CameraMovement::DOWN) m_Position.y += velocity;
            if (direction == CameraMovement::LEFT) m_Position.x -= velocity;
            if (direction == CameraMovement::RIGHT) m_Position.x += velocity;
        }

        void OrthoCamera::ProcessMouseScroll(float yoffset) {
            m_Zoom -= yoffset * 0.1f;
            if (m_Zoom < 0.1f) m_Zoom = 0.1f;
            if (m_Zoom > 10.0f) m_Zoom = 10.0f;
        }

    }  // namespace Playground2
}  // namespace Mir