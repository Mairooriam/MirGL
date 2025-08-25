#include "Playground/Playground.h"

#include <stb/stb_image.h>

#include "Playground.h"
#include "Primitives.h"
#include "imgui.h"
#include "log.h"
#include "magic_enum/magic_enum.hpp"
namespace Mir {

    Playground::Playground() = default;

    Playground::~Playground() {
        cleanup();
    }

    void Playground::setup() {
        if (m_queryID == 0) {
            glGenQueries(1, &m_queryID);
        }

        m_Camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 100.0f));
        m_orthoCamera = std::make_unique<Mir::OrthoCamera>(
            m_windowWidth, m_windowHeight, glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, -10.0f, 200.0f);
        m_shader = std::make_unique<Shader>("LightingBase.vs", "Lighting.fs");
        m_lightingShader = std::make_unique<Shader>("LightingBase.vs", "LightingLightCube.fs");

        Square square(glm::vec3(0, 0, 0), 300.0f);

        // Create another square at the origin with default size 1.0
        Square defaultSquare;

        objects_m.push_back(square);
        // objects_m.push_back(defaultSquare);

        for (const auto& object : objects_m) {
            objectsVertexData_m.insert(objectsVertexData_m.end(), object.vertices.begin(), object.vertices.end());
        }

        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(
            GL_ARRAY_BUFFER, objectsVertexData_m.size() * sizeof(Vertex), objectsVertexData_m.data(), GL_STATIC_DRAW);

        // Position attribute (location = 0)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Normal attribute (location = 1)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        glGenVertexArrays(1, &m_lightVAO);
        glBindVertexArray(m_lightVAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        setupFrameBuffers();
    }

    void Playground::setupFrameBuffers() {
        // Create framebuffer for perspective camera
        glGenFramebuffers(1, &m_perspectiveFramebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_perspectiveFramebuffer);

        // Create a texture to render to
        glGenTextures(1, &m_perspectiveTextureColorbuffer);
        glBindTexture(GL_TEXTURE_2D, m_perspectiveTextureColorbuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_windowWidth, m_windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_perspectiveTextureColorbuffer, 0);

        // Create a renderbuffer object for depth and stencil attachment
        glGenRenderbuffers(1, &m_perspectiveRBO);
        glBindRenderbuffer(GL_RENDERBUFFER, m_perspectiveRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_windowWidth, m_windowHeight);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_perspectiveRBO);

        // Check if the framebuffer is complete
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "ERROR::FRAMEBUFFER:: Perspective framebuffer is not complete!" << std::endl;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Playground::renderToFrameBuffer() {
        // Render perspective camera view to framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, m_perspectiveFramebuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 perspectiveView = m_Camera->GetViewMatrix();
        glm::mat4 perspectiveProjection =
            glm::perspective(glm::radians(m_Camera->GetZoom()), m_aspectRatio, 0.1f, 500.0f);

        m_shader->use();
        m_shader->setMat4("view", perspectiveView);
        m_shader->setMat4("projection", perspectiveProjection);

        glBindVertexArray(m_VAO);
        size_t vertexOffset = 0;
        for (const auto& object : objects_m) {
            glm::mat4 model = object.modelMatrix;
            m_shader->setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, vertexOffset, object.vertices.size());
            vertexOffset += object.vertices.size();
        }

        // Unbind framebuffer to render to the default framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Playground::handleInput() {
        if (m_updateMousePos) {
            // Automatically update mouse position
            glfwGetCursorPos(glfwGetCurrentContext(), &mouse_m.x_screen, &mouse_m.y_screen);
        } else {
            // Use manually set mouse position
            mouse_m.x_screen = m_manualMousePos.x;
            mouse_m.y_screen = m_manualMousePos.y;
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
            if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_S) == GLFW_PRESS)
                m_orthoCamera->ProcessKeyboard(CameraMovement::DOWN, m_deltaTime);
            if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_A) == GLFW_PRESS)
                m_orthoCamera->ProcessKeyboard(CameraMovement::LEFT, m_deltaTime);
            if (glfwGetKey(glfwGetCurrentContext(), GLFW_KEY_D) == GLFW_PRESS)
                m_orthoCamera->ProcessKeyboard(CameraMovement::RIGHT, m_deltaTime);
        }

    }

    void Playground::render() {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        double currentTime = glfwGetTime();
        m_deltaTime = currentTime - m_lastFrameTime;
        m_lastFrameTime = currentTime;

        glfwGetCursorPos(glfwGetCurrentContext(), &mouse_m.x_screen, &mouse_m.y_screen);

        renderToFrameBuffer();
        handleInput();

        glm::vec3 worldPos = ScreenToWorld(static_cast<float>(mouse_m.x_screen), static_cast<float>(mouse_m.y_screen));
        mouse_m.x_world = worldPos.x;
        mouse_m.y_world = worldPos.y;

        m_time += m_deltaTime;

        // glm::mat4 view = m_Camera->GetViewMatrix();
        // glm::mat4 projection = glm::perspective(glm::radians(m_Camera->GetZoom()), m_aspectRatio, 0.1f, 500.0f);

        glm::mat4 view = m_orthoCamera->GetViewMatrix();
        glm::mat4 projection = m_orthoCamera->GetProjectionMatrix();

        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::vec3 lightPos(1.1f, 1.0f, 1.5f);
        // glm::mat4 model = glm::mat4(1.0f);

        // std::cout << "Setting uniforms for main shader (ID: " << m_shader->m_id << ")" << std::endl;
        m_shader->use();
        m_shader->setMat4("view", view);
        m_shader->setMat4("projection", projection);
        m_shader->setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
        m_shader->setVec3("lightColor", glm::vec3(5.0f, 5.0f, 5.0f));
        m_shader->setVec3("lightPos", lightPos);
        m_shader->setVec3("viewPos", m_Camera->GetPosition());
        // m_shader->setMat4("model", model);

        glBindVertexArray(m_VAO);
        size_t vertexOffset = 0;
        for (const auto& object : objects_m) {
            glm::mat4 model = object.modelMatrix;
            m_shader->setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, vertexOffset, object.vertices.size());
            vertexOffset += object.vertices.size();
        }

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.1f));
        // std::cout << "Setting uniforms for light cube shader (ID: " << m_lightingShader->m_id << ")" << std::endl;
        m_lightingShader->use();
        m_lightingShader->setMat4("view", view);
        m_lightingShader->setMat4("projection", projection);
        m_lightingShader->setMat4("model", model);
        // Doesnt need to be set each frame but for now here :) since lightpos is in scope :))
        glBindVertexArray(m_lightVAO);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisable(GL_DEPTH_TEST);
    }

    void Playground::cleanup() {
        if (m_VAO) {
            glDeleteVertexArrays(1, &m_VAO);
            m_VAO = 0;
        }
        if (m_VBO) {
            glDeleteBuffers(1, &m_VBO);
            m_VBO = 0;
        }
        if (m_EBO) {
            glDeleteBuffers(1, &m_EBO);
            m_EBO = 0;
        }
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
    glm::vec3 Playground::ScreenToWorld(float x_screen, float y_screen) {
        // Step 1: Convert screen space to NDC
        float x_ndc = (2.0f * x_screen) / m_windowWidth - 1.0f;
        float y_ndc = 1.0f - (2.0f * y_screen) / m_windowHeight;  // Flip Y-axis
        debugData_m.ndc = glm::vec2(x_ndc, y_ndc);

        // Step 2: Convert NDC to world space
        glm::vec4 ndcPos(x_ndc, y_ndc, 0.0f, 1.0f);  // Z = 0 for 2D
        debugData_m.inverseProjection = glm::inverse(m_orthoCamera->GetProjectionMatrix());
        debugData_m.inverseView = glm::inverse(m_orthoCamera->GetViewMatrix());

        glm::vec4 worldPos = debugData_m.inverseProjection * debugData_m.inverseView * ndcPos;

        // Step 3: Return world coordinates
        debugData_m.worldPosition = glm::vec3(worldPos) / worldPos.w;

        return debugData_m.worldPosition;
    }

    void Playground::debugUI_CameraInfo() {
        // Perspective Camera Info
        {
            // Display important info in the dropdown title
            auto cameraState = m_Camera->GetCameraState();
            std::string perspectiveTitle = "Perspective Camera (Pos: " + std::to_string(cameraState.position.x) + ", " +
                std::to_string(cameraState.position.y) + ", " + std::to_string(cameraState.position.z) + ")";

            if (ImGui::TreeNode(perspectiveTitle.c_str())) {
                ImGui::Text("Perspective Camera Info:");
                ImGui::Text(
                    "Position: (%.2f, %.2f, %.2f)", cameraState.position.x, cameraState.position.y,
                    cameraState.position.z);
                ImGui::Text("Yaw: %.2f°", cameraState.yaw);
                ImGui::Text("Pitch: %.2f°", cameraState.pitch);
                ImGui::Text("Zoom: %.2f°", cameraState.zoom);
                ImGui::Text("Front: (%.2f, %.2f, %.2f)", cameraState.front.x, cameraState.front.y, cameraState.front.z);
                ImGui::Text("Up: (%.2f, %.2f, %.2f)", cameraState.up.x, cameraState.up.y, cameraState.up.z);
                ImGui::Text("Right: (%.2f, %.2f, %.2f)", cameraState.right.x, cameraState.right.y, cameraState.right.z);
                ImGui::Text("Movement Speed: %.2f", cameraState.movementSpeed);
                ImGui::Text("Mouse Sensitivity: %.3f", cameraState.mouseSensitivity);
                ImGui::TreePop();
            }
        }

        // Orthographic Camera Info
        {
            // Display important info in the dropdown title
            glm::vec3 orthoPos = m_orthoCamera->GetPosition();
            float orthoZoom = m_orthoCamera->GetZoom();
            std::string orthoTitle = "Orthographic Camera (Pos: " + std::to_string(orthoPos.x) + ", " +
                std::to_string(orthoPos.y) + ", Zoom: " + std::to_string(orthoZoom) + ")";

            if (ImGui::TreeNode(orthoTitle.c_str())) {
                ImGui::Text("Orthographic Camera Info:");
                ImGui::Text("Position: (%.2f, %.2f)", orthoPos.x, orthoPos.y);
                ImGui::Text("Zoom: %.2f", orthoZoom);
                ImGui::Text("Near Plane: %.2f", m_orthoCamera->GetNear());
                ImGui::Text("Far Plane: %.2f", m_orthoCamera->GetFar());
                ImGui::TreePop();
            }
        }
    }

    void Playground::debugUI_screenToWorld() {
        ImGui::Begin("Window Info");

        // Window dimensions and aspect ratio
        ImGui::Text("Window Dimensions:");
        ImGui::Text("Width: %d", m_windowWidth);
        ImGui::Text("Height: %d", m_windowHeight);
        ImGui::Text("Aspect Ratio: %.3f", m_aspectRatio);

        // Timing information
        ImGui::Separator();
        ImGui::Text("Timing Info:");
        ImGui::Text("Time: %.3f seconds", m_time);
        ImGui::Text("Delta Time: %.6f seconds", m_deltaTime);
        ImGui::Text("FPS: %.1f", 1.0 / m_deltaTime);

        // Mouse information
        ImGui::Separator();
        ImGui::Text("Mouse Info:");
        ImGui::Text("Screen Position: (%.1f, %.1f)", mouse_m.x_screen, mouse_m.y_screen);
        ImGui::Text("View Position: (%.2f, %.2f)", mouse_m.x_view, mouse_m.y_view);
        ImGui::Text("World Position: (%.2f, %.2f)", mouse_m.x_world, mouse_m.y_world);

        // Debug data (ScreenToWorld)
        ImGui::Separator();
        ImGui::Text("ScreenToWorld Debug Data:");
        ImGui::Text("NDC Coordinates: (%.3f, %.3f)", debugData_m.ndc.x, debugData_m.ndc.y);

        ImGui::Text("Inverse Projection Matrix:");
        for (int i = 0; i < 4; ++i) {
            ImGui::Text(
                "[%.3f, %.3f, %.3f, %.3f]", debugData_m.inverseProjection[i][0], debugData_m.inverseProjection[i][1],
                debugData_m.inverseProjection[i][2], debugData_m.inverseProjection[i][3]);
        }

        ImGui::Text("Inverse View Matrix:");
        for (int i = 0; i < 4; ++i) {
            ImGui::Text(
                "[%.3f, %.3f, %.3f, %.3f]", debugData_m.inverseView[i][0], debugData_m.inverseView[i][1],
                debugData_m.inverseView[i][2], debugData_m.inverseView[i][3]);
        }

        ImGui::Text("World Position:");
        ImGui::Text(
            "X: %.3f, Y: %.3f, Z: %.3f", debugData_m.worldPosition.x, debugData_m.worldPosition.y,
            debugData_m.worldPosition.z);

        ImGui::End();
    }

    void Playground::renderUI() {
        ImGui::Begin("Example Settings");
        ImGui::Text("Current state %s", magic_enum::enum_name(m_activeWindow).data());

        debugUI_CameraInfo();
        debugUI_screenToWorld();

        ImGui::Checkbox("Update Mouse Position", &m_updateMousePos);

        // Manual mouse position input
        if (!m_updateMousePos) {
            ImGui::InputFloat2("Manual Mouse Position", &m_manualMousePos.x);
        }

        ImGui::Separator();
        ImGui::Text("Time: %.2f", m_time);
        ImGui::Text("Delta Time: %.6f", m_deltaTime);
        ImGui::Text("FPS: %.1f", 1.0f / m_deltaTime);

        ImGui::Separator();
        ImGui::Text("Mouse Info");
        ImGui::Text("Window Position: (%.1f,%.1f)", mouse_m.x_screen, mouse_m.y_screen);
        ImGui::Text("View Position: (%.2f,%.2f)", mouse_m.x_view, mouse_m.y_view);
        ImGui::Text("World Position: (%.2f,%.2f)", mouse_m.x_world, mouse_m.y_world);

        // Camera info
        ImGui::Separator();
        ImGui::Text("Camera Controls:");
        ImGui::Text("WASD - Move camera");
        ImGui::Text("Mouse - Look around");
        ImGui::Text("Scroll - Zoom");

        ImGui::Separator();
        ImGui::Text("OpenGL State:");
        ImGui::Text("Depth Test: %s", glIsEnabled(GL_DEPTH_TEST) ? "ON" : "OFF");
        ImGui::Text("Face Culling: %s", glIsEnabled(GL_CULL_FACE) ? "ON" : "OFF");
        ImGui::Text("Blend: %s", glIsEnabled(GL_BLEND) ? "ON" : "OFF");
        ImGui::Separator();
        ImGui::Text("Camera info");

        ImGui::End();

        ImGui::Begin("Perspective Camera View");

        ImGui::Text("Perspective Camera View:");
        ImGui::Image((void*)(intptr_t)m_perspectiveTextureColorbuffer, ImVec2(400, 300));  // Adjust size as needed
        if (ImGui::Button("Control Main Window")) {
            m_activeWindow = ActiveWindow::MAIN_VIEWPORT;
        }
        if (ImGui::Button("Control Second Window")) {
            m_activeWindow = ActiveWindow::SECOND_VIEWPORT;
        }
        ImGui::End();
    }
}  // namespace Mir