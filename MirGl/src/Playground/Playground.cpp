#include "Playground/Playground.h"

#include <stb/stb_image.h>

#include "Collision.h"
#include "DebugUI.h"
#include "Playground.h"
#include "Primitives.h"
#include "imgui.h"
#include "log.h"
#include "magic_enum/magic_enum.hpp"

namespace Mir {

    void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
        (void)xoffset;
        auto* playground = static_cast<Mir::Playground*>(glfwGetWindowUserPointer(window));

        if (playground->useOrthoCamera) {
            playground->getOrthoCamera()->ProcessMouseScroll(static_cast<float>(yoffset));
        } else {
            playground->getCamera()->ProcessMouseScroll(static_cast<float>(yoffset));
        }
    }

    Playground::Playground() = default;

    void Playground::updateDebugData() {
        // Update perspective camera info
        dData_m.perspectiveCamera.position = m_Camera->GetPosition();
        dData_m.perspectiveCamera.front = m_Camera->GetFront();
        dData_m.perspectiveCamera.up = m_Camera->GetUp();
        dData_m.perspectiveCamera.right = m_Camera->GetRight();
        dData_m.perspectiveCamera.yaw = m_Camera->GetYaw();
        dData_m.perspectiveCamera.pitch = m_Camera->GetPitch();
        dData_m.perspectiveCamera.zoom = m_Camera->GetZoom();

        // Update orthographic camera info
        dData_m.orthoCamera.position = m_orthoCamera->GetPosition();
        dData_m.orthoCamera.front = glm::vec3(0.0f, 0.0f, -1.0f);  // Fixed for ortho camera
        dData_m.orthoCamera.up = glm::vec3(0.0f, 1.0f, 0.0f);      // Fixed for ortho camera
        dData_m.orthoCamera.right = glm::vec3(1.0f, 0.0f, 0.0f);   // Fixed for ortho camera
        dData_m.orthoCamera.yaw = 0.0f;                            // Not applicable for ortho camera
        dData_m.orthoCamera.pitch = 0.0f;                          // Not applicable for ortho camera
        dData_m.orthoCamera.zoom = m_orthoCamera->GetZoom();

        // Update mouse info
        dData_m.mouse = &mouse_m;

        // Update timing info
        dData_m.deltaTime = m_deltaTime;
        dData_m.time = m_time;
        dData_m.fps = (m_deltaTime > 0.0f) ? 1.0f / m_deltaTime : 0.0f;

        // Update window info
        dData_m.windowWidth = m_windowWidth;
        dData_m.windowHeight = m_windowHeight;
        dData_m.aspectRatio = m_aspectRatio;

        // Update debug matrices
        dData_m.inverseProjection = glm::inverse(
            useOrthoCamera ? m_orthoCamera->GetProjectionMatrix()
                           : glm::perspective(glm::radians(m_Camera->GetZoom()), m_aspectRatio, 0.1f, 500.0f));
        dData_m.inverseView = glm::inverse(useOrthoCamera ? m_orthoCamera->GetViewMatrix() : m_Camera->GetViewMatrix());

        // General debug info
        if (dData_m.showWireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        if (dData_m.MainWindow) {
            m_activeWindow = ActiveWindow::MAIN_VIEWPORT;
        } else {
            m_activeWindow = ActiveWindow::SECOND_VIEWPORT;
        }

        // OBJECTS
        dData_m.objects = &objects_m;

        // LIGHTS
        dData_m.lights = &lights_m;
    }

    Playground::~Playground() {
        cleanup();
    }

    void Playground::setup() {
        if (m_queryID == 0) {
            glGenQueries(1, &m_queryID);
        }

        glEnable(GL_PROGRAM_POINT_SIZE);

        m_Camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 10.0f));
        m_orthoCamera = std::make_unique<Mir::OrthoCamera>(
            m_windowWidth, m_windowHeight, glm::vec3(0.0f, 0.0f, 3.0f), 0.2f, -1.0f, 10.0f);
        m_shader = std::make_unique<Shader>("Playground.vs", "Lighting.fs");
        m_lightingShader = std::make_unique<Shader>("Playground.vs", "PlaygroundLight.fs");
        dbUI_m.setData(&dData_m);

        // Create objects and assign unique IDs
        objects_m.clear();
        objects_m.emplace_back(Square(glm::vec3(-10.0f, -10.0f, 0.0f), 10.0f));  // Object 1
        objects_m.emplace_back(Square(glm::vec3(10.0f, 0.0f, 0.0f), 10.0f));     // Object 2
        objects_m.emplace_back(Square(glm::vec3(0.0f, 10.0f, 0.0f), 10.0f));     // Object 3
        objects_m.emplace_back(Circle(5.0, 8));

        objects_m[0].drawMode = DrawMode::Points;

        lights_m.clear();
        // Define parameters for the first light
        glm::vec3 light1Position = glm::vec3(5.0f, 5.0f, 3.0f);  // Z = 1.0f for visibility
        glm::vec3 light1Color = glm::vec3(1.0f, 1.0f, 1.8f);
        float light1Intensity = 1.0f;
        auto lightMesh1 = Square(light1Position, 1.0f);
        Light::Controls light1Controls{light1Intensity, light1Color};
        lights_m.push_back(Light(light1Controls, lightMesh1));

        // Define parameters for the second light
        // glm::vec3 light2Position = glm::vec3(-5.0f, -5.0f, 3.0f);
        // glm::vec3 light2Color = glm::vec3(0.8f, 1.0f, 1.0f);
        // float light2Intensity = 0.7f;
        // auto lightMesh2 = Circle(2.0f, 16, light2Position);
        // Light::Controls light2Controls{light2Intensity, light2Color};
        // lights_m.push_back(Light(light2Controls, lightMesh2));

        std::vector<Vertex> combinedVertices;
        size_t vertexOffset = 0;
        for (auto& object : objects_m) {
            vertexOffset += object.vertices.size();
            combinedVertices.insert(combinedVertices.end(), object.vertices.begin(), object.vertices.end());
        }

        // Setup main VBO and VAO
        m_VAO = std::make_unique<Mir::VAO>();
        m_VBO = std::make_unique<Mir::VBO>();

        m_VAO->bind();
        m_VBO->bind();
        m_VBO->setData(combinedVertices);

        VertexLayouts layout = {
            {0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, position)},
            {1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, normal)}};
        m_VAO->setupVertexAttributes(layout);

        m_VBO->unbind();
        m_VAO->unbind();

        glfwSetScrollCallback(glfwGetCurrentContext(), ScrollCallback);
        glfwSetWindowUserPointer(glfwGetCurrentContext(), this);
    }

    void Playground::handleInput() {
        if (m_updateMousePos) {
            bool leftPressed = glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
            bool rightPressed = glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
            bool middlePressed = glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS;

            mouse_m.updateState(leftPressed, rightPressed, middlePressed);

            glfwGetCursorPos(glfwGetCurrentContext(), &mouse_m.screen.x, &mouse_m.screen.y);
            glm::vec3 worldPos = ScreenToWorld(mouse_m.screen.x, mouse_m.screen.y);
            mouse_m.world.x = worldPos.x;
            mouse_m.world.y = worldPos.y;

        } else {
            // Use manually set mouse position
            mouse_m.screen.x = m_manualMousePos.x;
            mouse_m.screen.y = m_manualMousePos.y;
        }

        if (m_activeWindow == ActiveWindow::SECOND_VIEWPORT) {
            if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_W) == GLFW_PRESS)
                m_Camera->ProcessKeyboard(CameraMovement::FORWARD, m_deltaTime);
            if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_S) == GLFW_PRESS)
                m_Camera->ProcessKeyboard(CameraMovement::BACKWARD, m_deltaTime);
            if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_A) == GLFW_PRESS)
                m_Camera->ProcessKeyboard(CameraMovement::LEFT, m_deltaTime);
            if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_D) == GLFW_PRESS)
                m_Camera->ProcessKeyboard(CameraMovement::RIGHT, m_deltaTime);
            if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_E) == GLFW_PRESS)
                m_Camera->ProcessKeyboard(CameraMovement::ROTATE_RIGHT, m_deltaTime);
            if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_Q) == GLFW_PRESS)
                m_Camera->ProcessKeyboard(CameraMovement::ROTATE_LEFT, m_deltaTime);
            if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_C) == GLFW_PRESS)
                m_Camera->ProcessKeyboard(CameraMovement::ROTATE_UP, m_deltaTime);
            if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_X) == GLFW_PRESS)
                m_Camera->ProcessKeyboard(CameraMovement::ROTATE_DOWN, m_deltaTime);
        } else if (m_activeWindow == ActiveWindow::MAIN_VIEWPORT) {
            if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_W) == GLFW_PRESS)
                m_orthoCamera->ProcessKeyboard(CameraMovement::UP, m_deltaTime);
            if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_S) == GLFW_PRESS) {
                m_orthoCamera->ProcessKeyboard(CameraMovement::DOWN, m_deltaTime);
            }
            if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_A) == GLFW_PRESS)
                m_orthoCamera->ProcessKeyboard(CameraMovement::LEFT, m_deltaTime);
            if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_D) == GLFW_PRESS)
                m_orthoCamera->ProcessKeyboard(CameraMovement::RIGHT, m_deltaTime);
        }
    }

    void Playground::render() {
        glEnable(GL_BLEND);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        updateTime();
        handleInput();
        updateDebugData();

        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set up camera, shader, etc. here if needed

        // --- Main viewport ---
        glViewport(0, 0, m_windowWidth, m_windowHeight);

        glm::mat4 view = useOrthoCamera ? m_orthoCamera->GetViewMatrix() : m_Camera->GetViewMatrix();
        glm::mat4 projection = useOrthoCamera
            ? m_orthoCamera->GetProjectionMatrix()
            : glm::perspective(glm::radians(m_Camera->GetZoom()), m_aspectRatio, 0.1f, 500.0f);

        if (!lights_m.empty()) {
            m_shader->use();
            m_shader->setVec3("lightPos", glm::vec3(lights_m[0].mesh.modelMatrix[3]));
            m_shader->setVec3("lightColor", lights_m[0].getLightColor());
            // m_shader->setFloat("lightIntensity", lights_m[0].controls.intensity);
        }

        checkCollision(objects_m, view, projection);
        drawObjects(view, projection);  // This uses your main camera (ortho or perspective)
        drawLights(view, projection);
        // --- Minimap viewport setup ---
        int minimapWidth = m_windowWidth / 4;
        int minimapHeight = m_windowHeight / 4;
        int minimapX = m_windowWidth - minimapWidth - 10;  // 10px from right
        int minimapY = 10;                                 // 10px from bottom

        glViewport(minimapX, minimapY, minimapWidth, minimapHeight);

        // Set up perspective camera for minimap
        glm::mat4 minimapView = m_Camera->GetViewMatrix();
        glm::mat4 minimapProjection = glm::perspective(
            glm::radians(m_Camera->GetZoom()), static_cast<float>(minimapWidth) / minimapHeight, 0.1f, 500.0f);

        drawObjects(minimapView, minimapProjection);
        drawLights(minimapView, minimapProjection);
    }

    void Playground::drawObject(const Object& object, size_t& offset) {
        switch (object.drawMode) {
            case DrawMode::Triangles:
                glDrawArrays(GL_TRIANGLES, offset, object.vertices.size());
                offset += object.vertices.size();
                break;
            case DrawMode::TriangleFan:
                glDrawArrays(GL_TRIANGLE_FAN, offset, object.vertices.size());
                offset += object.vertices.size();
                break;
            case DrawMode::Lines:
                glDrawArrays(GL_LINES, offset, object.vertices.size());
                offset += object.vertices.size();
                break;
            case DrawMode::Points:
                glDrawArrays(GL_POINTS, offset, object.vertices.size());
                offset += object.vertices.size();
                break;

            case DrawMode::IndexedTriangles:
                // If using indices and EBO, call glDrawElements here
                // glDrawElements(GL_TRIANGLES, object.indices.size(), GL_UNSIGNED_INT, ...);
                break;
        }
    }

    void Playground::drawObjects(const glm::mat4& view, const glm::mat4& projection) {
        m_shader->use();
        m_shader->setMat4("view", view);
        m_shader->setMat4("projection", projection);
        if (useOrthoCamera) {
            m_shader->setVec3("viewPos", m_orthoCamera->GetPosition());
        } else {
            m_shader->setVec3("viewPos", m_Camera->GetPosition());
        }

        // --- Use VAO abstraction for geometry ---
        m_VAO->bind();
        size_t vertexOffset = 0;
        for (const auto& object : objects_m) {
            size_t objectStartOffset = vertexOffset;
            m_shader->setMat4("model", object.modelMatrix);
            m_shader->setVec3("objectColor", object.color);
            drawObject(object, vertexOffset);
            if (object.isSelected) {
                glDisable(GL_DEPTH_TEST);
                for (const auto& vertex : object.vertices) {
                    if (vertex.selected) {
                        size_t vertexIndex = &vertex - &object.vertices[0];
                        size_t globalIndex = objectStartOffset + vertexIndex;  // Correct calculation
                        glm::mat4 model = object.modelMatrix;
                        m_shader->setMat4("model", model);
                        m_shader->setVec3("objectColor", glm::vec3(1.0f, 0.0f, 0.0f));
                        glDrawArrays(GL_POINTS, globalIndex, 1);
                    }
                }
                glEnable(GL_DEPTH_TEST);
            }
        }

        // // --- Use VAO abstraction for lighting ---
        // glm::mat4 model = glm::mat4(1.0f);
        // model = glm::translate(model, m_lightPosition);
        // model = glm::scale(model, glm::vec3(0.1f));
        // m_lightingShader->use();
        // m_lightingShader->setMat4("view", view);
        // m_lightingShader->setMat4("projection", projection);
        // m_lightingShader->setMat4("model", model);

        // m_lightVAO->bind();
        // glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void Playground::drawLights(const glm::mat4& view, const glm::mat4& projection) {
        m_lightingShader->use();
        m_lightingShader->setMat4("view", view);
        m_lightingShader->setMat4("projection", projection);

        for (const auto& light : lights_m) {
            m_lightingShader->setMat4("model", light.mesh.modelMatrix);
            m_lightingShader->setVec3("objectColor", light.controls.color);
            m_VAO->bind();
            glDrawArrays(GL_TRIANGLES, 0, light.mesh.vertices.size());
        }
    }

    void Playground::checkCollision(std::vector<Object>& objects, const glm::mat4& view, const glm::mat4& projection) {
        for (auto& object : objects) {
            bool anyVertexSelected = false;
            for (auto& vertex : object.vertices) {
                if (Mir::isPointSelected(
                        vertex.position,
                        object.modelMatrix,
                        view,
                        projection,
                        m_windowWidth,
                        m_windowHeight,
                        mouse_m.screen.x,
                        mouse_m.screen.y,
                        5.0f  // selection radius in pixels
                        )) {
                    printf(
                        "Colliding with object (%i), vertex (x,y,z): (%0.1f, %0.1f, %0.1f)\n",
                        object.id,
                        vertex.position.x,
                        vertex.position.y,
                        vertex.position.z);
                    vertex.selected = true;
                    anyVertexSelected = true;
                } else {
                    vertex.selected = false;
                }


                if (anyVertexSelected)
                {
                    object.isSelected = true;
                }else{
                    object.isSelected = false;
                }
                
                // Handle selection: highlight, store index, etc.
            }
        }
    }

    void Playground::cleanup() {
        if (m_texture1) {
            glDeleteTextures(1, &m_texture1);
            m_texture1 = 0;
        }
        if (m_texture2) {
            glDeleteTextures(1, &m_texture2);

            m_texture2 = 0;
        }
        if (m_queryID != 0) {
            glDeleteQueries(1, &m_queryID);
            m_queryID = 0;
        }
        if (m_perspectiveFramebuffer) {
            glDeleteFramebuffers(1, &m_perspectiveFramebuffer);
            m_perspectiveFramebuffer = 0;
        }
        if (m_perspectiveTextureColorbuffer) {
            glDeleteTextures(1, &m_perspectiveTextureColorbuffer);
            m_perspectiveTextureColorbuffer = 0;
        }
        if (m_perspectiveRBO) {
            glDeleteRenderbuffers(1, &m_perspectiveRBO);
            m_perspectiveRBO = 0;
        }

        m_shader.reset();
    }
    void Playground::updateTime() {
        double currentTime = glfwGetTime();
        m_deltaTime = currentTime - m_lastFrameTime;
        m_lastFrameTime = currentTime;
        m_time += m_deltaTime;
    }
    glm::vec3 Playground::ScreenToWorld(float x_screen, float y_screen) {
        if (useOrthoCamera) {
            return m_orthoCamera->ScreenToWorld(x_screen, y_screen, m_windowWidth, m_windowHeight);
        } else {
            return glm::vec3();
        }
    }

    void Playground::renderUI() {
        dbUI_m.render();
    }

}  // namespace Mir