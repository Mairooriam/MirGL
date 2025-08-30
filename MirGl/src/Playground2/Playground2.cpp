#include "Playground2.h"

#include <memory>

#include "Draw.h"
#include "PlayGroundCamera.h"

namespace Mir {
    namespace Playground2 {

        Playground2::Playground2() {}
        Playground2::~Playground2() {}
        void Playground2::setup() {
            cameras_m.emplace_back(std::make_shared<PerspectiveCamera>(glm::vec3(0.0f, 0.0f, 10.0f)));
            cameras_m.emplace_back(
                std::make_shared<OrthoCamera>(800.0f, 600.0f, glm::vec3(0.0f, 0.0f, 3.0f), 0.2f, 1.0f, 100.0f));
            shaders_m["main"] = std::make_unique<Shader>("Playground.vs", "Lighting.fs");
            shaders_m["light"] = std::make_unique<Shader>("Playground.vs", "LightingLightCube.fs");

            objects_m.emplace_back(Circle(glm::vec3(0.0f, 0.0f, 0.0f), 5.0f, Color(255, 0, 0, 255)));
            objects_m.emplace_back(Rectangle(-2.0f, -1.0f, 4.0f, 2.0f));
            std::vector<Vertex> combinedVertices;

            // Iterate over all objects and collect their vertices
            for (auto&& ob : objects_m) {
                std::visit(
                    [&](auto&& shape) {
                        combinedVertices.insert(combinedVertices.end(), shape.vertices.begin(), shape.vertices.end());
                    },
                    ob);
            }

            glGenVertexArrays(1, &m_VAO);
            glGenBuffers(1, &m_VBO);

            glBindVertexArray(m_VAO);
            glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
            glBufferData(
                GL_ARRAY_BUFFER, combinedVertices.size() * sizeof(Vertex), combinedVertices.data(), GL_STATIC_DRAW);

            // Position attribute (location = 0)
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
            glEnableVertexAttribArray(0);

            // Normal attribute (location = 1)
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
            glEnableVertexAttribArray(1);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);

            // glGenVertexArrays(1, &m_lightVAO);
            // glBindVertexArray(m_lightVAO);
            // glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
            // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
            // glEnableVertexAttribArray(0);
            // glBindVertexArray(0);
        }
        void Playground2::cleanup() {}
        void Playground2::renderUI() {}
        void Playground2::handleInput() {
            double xpos, ypos;
            glfwGetCursorPos(glfwGetCurrentContext(), &xpos, &ypos);
            mouse_m.screen_pos.x = static_cast<float>(xpos);
            mouse_m.screen_pos.y = static_cast<float>(ypos);


            auto camera = getCurrentCamera();
            if (controlState_m == ControlState::ORTHO_CAMERA) {
                if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_W) == GLFW_PRESS)
                    camera->ProcessKeyboard(CameraMovement::UP, m_deltaTime);
                if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_S) == GLFW_PRESS) {
                    camera->ProcessKeyboard(CameraMovement::DOWN, m_deltaTime);
                }
                if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_A) == GLFW_PRESS)
                    camera->ProcessKeyboard(CameraMovement::LEFT, m_deltaTime);
                if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_D) == GLFW_PRESS)
                    camera->ProcessKeyboard(CameraMovement::RIGHT, m_deltaTime);
            } else if (controlState_m == ControlState::PERSPECTIVE_CAMERA) {
                if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_W) == GLFW_PRESS)
                    camera->ProcessKeyboard(CameraMovement::FORWARD, m_deltaTime);
                if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_S) == GLFW_PRESS)
                    camera->ProcessKeyboard(CameraMovement::BACKWARD, m_deltaTime);
                if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_A) == GLFW_PRESS)
                    camera->ProcessKeyboard(CameraMovement::LEFT, m_deltaTime);
                if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_D) == GLFW_PRESS)
                    camera->ProcessKeyboard(CameraMovement::RIGHT, m_deltaTime);
                if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_E) == GLFW_PRESS)
                    camera->ProcessKeyboard(CameraMovement::ROTATE_RIGHT, m_deltaTime);
                if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_Q) == GLFW_PRESS)
                    camera->ProcessKeyboard(CameraMovement::ROTATE_LEFT, m_deltaTime);
                if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_C) == GLFW_PRESS)
                    camera->ProcessKeyboard(CameraMovement::ROTATE_UP, m_deltaTime);
                if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_X) == GLFW_PRESS)
                    camera->ProcessKeyboard(CameraMovement::ROTATE_DOWN, m_deltaTime);
            }
        }

        void Playground2::render() {
            glEnable(GL_BLEND);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            Shader& shaderProg = *shaders_m.find("main")->second;
            size_t initialOffset = 0;
            double currentTime = glfwGetTime();
            m_deltaTime = currentTime - m_lastFrameTime;
            m_lastFrameTime = currentTime;

            handleInput();

            auto camera = getCurrentCamera();
            glm::mat4 view = camera->GetViewMatrix();
            glm::mat4 projection = camera->GetProjectionMatrix();
            glBindVertexArray(m_VAO);

            // Pass the precomputed matrices to the Draw struct
            Draw draw(shaderProg, initialOffset, view, projection);

            for (const auto& ob : objects_m) {
                std::visit(draw, ob);
            }
            glDisable(GL_DEPTH_TEST);
        }
    }  // namespace Playground2
}  // namespace Mir