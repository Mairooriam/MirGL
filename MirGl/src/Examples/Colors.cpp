#include "Examples/Colors.h"
#include "imgui.h"
#include "log.h"
#include <stb/stb_image.h>


namespace Mir {

Colors::Colors() = default;

Colors::~Colors() {
    cleanup();
}

void Colors::setup() {
    if (m_queryID == 0) { 
        glGenQueries(1, &m_queryID);
    }
    
    m_Camera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));

    m_shader = std::make_unique<Shader>("Colors.vs", "Colors2.fs");
    m_lightingShader = std::make_unique<Shader>("Colors.vs", "Colors.fs");
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f
    };


    m_cubePositions = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };
    
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);

    
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenVertexArrays(1, &m_lightVAO);
    glBindVertexArray(m_lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void Colors::render() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    
    double currentTime = glfwGetTime();
    m_deltaTime = currentTime - m_lastFrameTime;
    m_lastFrameTime = currentTime;
    
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

    m_time += m_deltaTime;
    
    double totalDuration = m_cubePositions.size() * m_delay;
    if (m_time >= totalDuration) {
        m_time = 0.0f;
    }

    glm::mat4 view = m_Camera->GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(m_Camera->GetZoom()), m_aspectRatio, 0.1f, 100.0f);
    
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // RENDER MAIN CUBE
    m_shader->use();
    m_shader->setMat4("view", view);
    m_shader->setMat4("projection", projection);

    
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
    m_shader->setMat4("model", model);
    
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // RENDER LIGHT CUBE
    m_lightingShader->use();
    m_lightingShader->setMat4("view", view);
    m_lightingShader->setMat4("projection", projection);
    m_lightingShader->setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f)); // Orange
    m_lightingShader->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));   // White light
    
    glm::vec3 lightPos(1.1f, 1.0f, 1.5f);
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.5f));
    model = glm::rotate(model, (float)glfwGetTime() * glm::radians(-50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
    m_lightingShader->setMat4("model", model);
    
    glBindVertexArray(m_lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

 
    
    glDisable(GL_DEPTH_TEST);

}

void Colors::cleanup() {
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
    m_shader.reset();
}
void Colors::renderUI(){
    ImGui::Begin("Example Settings");
    
    // Basic settings
    ImGui::SliderFloat("Fade", &m_fade, 0.0f, 1.0f, "%.2f");
    ImGui::SliderFloat("Aspect-Ratio", &m_aspectRatio, 0.0f, 10.0f, "%.2f");
    ImGui::SliderFloat("Delay", &m_delay, 0.0f, 10.0f, "%.2f");

    ImGui::Separator();
    ImGui::Text("Time: %.2f", m_time);
    ImGui::Text("Delta Time: %.6f", m_deltaTime);
    ImGui::Text("FPS: %.1f", 1.0f / m_deltaTime);
    
    // Camera info
    ImGui::Separator();
    ImGui::Text("Camera Controls:");
    ImGui::Text("WASD - Move camera");
    ImGui::Text("Mouse - Look around");
    ImGui::Text("Scroll - Zoom");
    
    glm::vec3 pos = m_Camera->GetPosition();
    ImGui::Text("Camera Position: (%.2f, %.2f, %.2f)", pos.x, pos.y, pos.z);
    ImGui::Text("Camera Zoom: %.2f", m_Camera->GetZoom());

    if (ImGui::Button("reset"))
    {
        m_time = 0.0f;
    }
    if (ImGui::Button("Add delay to timer"))
    {
        m_time += m_delay;
    }
    
    ImGui::Separator();
    ImGui::RadioButton("one box", (int*)&m_rMode, (int)renderMode::SingleBox);
    ImGui::RadioButton("10 box", (int*)&m_rMode, (int)renderMode::MultipleBoxes);
    ImGui::Separator();
    ImGui::Text("OpenGL State:");
    ImGui::Text("Depth Test: %s", glIsEnabled(GL_DEPTH_TEST) ? "ON" : "OFF");
    ImGui::Text("Face Culling: %s", glIsEnabled(GL_CULL_FACE) ? "ON" : "OFF");
    ImGui::Text("Blend: %s", glIsEnabled(GL_BLEND) ? "ON" : "OFF");
    ImGui::Separator();
    
    auto cameraState = m_Camera->GetCameraState();
    
    ImGui::Text("Position: (%.2f, %.2f, %.2f)", 
                cameraState.position.x, cameraState.position.y, cameraState.position.z);
    ImGui::Text("Front: (%.2f, %.2f, %.2f)", 
                cameraState.front.x, cameraState.front.y, cameraState.front.z);
    ImGui::Text("Up: (%.2f, %.2f, %.2f)", 
                cameraState.up.x, cameraState.up.y, cameraState.up.z);
    ImGui::Text("Right: (%.2f, %.2f, %.2f)", 
                cameraState.right.x, cameraState.right.y, cameraState.right.z);
    ImGui::Text("Yaw: %.2f°", cameraState.yaw);
    ImGui::Text("Pitch: %.2f°", cameraState.pitch);
    ImGui::Text("Zoom: %.2f°", cameraState.zoom);
    ImGui::Text("Movement Speed: %.2f", cameraState.movementSpeed);
    ImGui::Text("Mouse Sensitivity: %.3f", cameraState.mouseSensitivity);

    
    ImGui::End();
}
} // namespace Mir