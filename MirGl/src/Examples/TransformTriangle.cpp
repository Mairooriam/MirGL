#include "Examples/TransformTriangle.h"
#include "imgui.h"
#include "log.h"
#include <stb/stb_image.h>


namespace Mir {

TransformTriangle::TransformTriangle() = default;

TransformTriangle::~TransformTriangle() {
    cleanup();
}

void TransformTriangle::setup() {
    if (m_queryID == 0) { 
        glGenQueries(1, &m_queryID);
    }
    

    glGenTextures(1, &m_texture1);
    glBindTexture(GL_TEXTURE_2D, m_texture1);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load and generate the texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    glGenTextures(1, &m_texture2);
    glBindTexture(GL_TEXTURE_2D, m_texture2);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width2, height2, nrChannels2;
    stbi_set_flip_vertically_on_load(true); 
    unsigned char *data2 = stbi_load("awesomeface.png", &width2, &height2, &nrChannels2, 0);
    if (data2)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, data2);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data2);

    m_shader = std::make_unique<Shader>("TransformTriangle.vs", "TransformTriangle.fs");
    
    float vertices[] = {
        // positions          // colors           // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };
    
    unsigned int indices[] = {
        0, 1, 3,   
        1, 2, 3    
    };
    
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);  // Add this line
    
    glBindVertexArray(m_VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // Set up EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // Position attribute (3 floats)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0);

    // Color attribute (3 floats)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture coordinate attribute (2 floats)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void TransformTriangle::render() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBeginQuery(GL_TIME_ELAPSED, m_queryID);

    m_trans = glm::mat4(1.0f); 
    glm::mat4 secondTrans = glm::mat4(1.0f);
    if (m_animate) {
        float currentTime = static_cast<float>(glfwGetTime() * m_speedMultiplier);
        secondTrans = glm::scale(secondTrans, glm::vec3(sin(currentTime))); 
        // Calculate X coordinate based on selected wave type
        float x = 0.0f;
        switch (m_xWaveType) {
            case WaveType::Sine:
                x = sin(currentTime) * m_xMultiplier;
                break;
            case WaveType::Cosine:
                x = cos(currentTime) * m_xMultiplier;
                break;
            case WaveType::Tangent:
                x = std::max(-2.0f, std::min(2.0f, static_cast<float>(tan(currentTime)))) * m_xMultiplier * 0.2f;
                break;
        }
        
        // Calculate Y coordinate based on selected wave type
        float y = 0.0f;
        switch (m_yWaveType) {
            case WaveType::Sine:
                y = sin(currentTime) * m_yMultiplier;
                break;
            case WaveType::Cosine:
                y = cos(currentTime) * m_yMultiplier;
                break;
            case WaveType::Tangent:
                // Limit tangent to avoid extreme values
                y = std::max(-2.0f, std::min(2.0f, static_cast<float>(tan(currentTime)))) * m_yMultiplier * 0.2f;
                break;
        }
        
        m_trans = glm::translate(m_trans, glm::vec3(x, y, 0.0f));
        
        float autoRotation = fmod(currentTime * 45.0f, 360.0f); // 45 degrees per second
        m_trans = glm::rotate(m_trans, glm::radians(autoRotation), glm::vec3(0.0f, 0.0f, 1.0f));
    }
   
    m_trans = glm::rotate(m_trans, glm::radians(m_rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    m_trans = glm::scale(m_trans, m_scale);
    
    m_shader->use();
    m_shader->setInt("texture1", 0);
    m_shader->setInt("texture2", 1);
    m_shader->setFloat("fade", m_fade);
    m_shader->setMat4("trans", m_trans);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_texture2);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture1);
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
    
    secondTrans = glm::translate(secondTrans, glm::vec3(-0.7f, 0.7f, 0.0f)); // Different position
    secondTrans = glm::rotate(secondTrans, glm::radians(-m_rotation), glm::vec3(0.0f, 0.0f, 1.0f)); // Opposite rotation
    secondTrans = glm::scale(secondTrans, glm::vec3(0.3f)); // Smaller scale
    
    m_shader->setMat4("trans", secondTrans);
    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glEndQuery(GL_TIME_ELAPSED);
    
    GLint queryResultAvailable = 0;
    glGetQueryObjectiv(m_queryID, GL_QUERY_RESULT_AVAILABLE, &queryResultAvailable);
    
    if (queryResultAvailable) {
        GLuint64 timeElapsed = 0;
        glGetQueryObjectui64v(m_queryID, GL_QUERY_RESULT, &timeElapsed);
        
        m_lastRenderTimeMs = timeElapsed / 1000000.0f;
    }
}

void TransformTriangle::cleanup() {
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
void TransformTriangle::renderUI(){
    ImGui::Begin("Example Settings");
    
    // Basic settings
    ImGui::SliderFloat("Fade", &m_fade, 0.0f, 1.0f, "%.2f");
    ImGui::SliderFloat("Rotation", &m_rotation, 0.0f, 360.0f, "%.2f"); // Fixed typo
    ImGui::Checkbox("Animate", &m_animate);
    
    // Wave animation settings
    if (m_animate) {
        ImGui::Separator();
        ImGui::Text("Animation Settings");
        
        // X wave type selection
        ImGui::Text("X Coordinate Wave:");
        int xWaveType = static_cast<int>(m_xWaveType);
        if (ImGui::RadioButton("Sine##X", &xWaveType, 0)) m_xWaveType = WaveType::Sine;
        ImGui::SameLine();
        if (ImGui::RadioButton("Cosine##X", &xWaveType, 1)) m_xWaveType = WaveType::Cosine;
        ImGui::SameLine();
        if (ImGui::RadioButton("Tangent##X", &xWaveType, 2)) m_xWaveType = WaveType::Tangent;
        
        // Y wave type selection
        ImGui::Text("Y Coordinate Wave:");
        int yWaveType = static_cast<int>(m_yWaveType);
        if (ImGui::RadioButton("Sine##Y", &yWaveType, 0)) m_yWaveType = WaveType::Sine;
        ImGui::SameLine();
        if (ImGui::RadioButton("Cosine##Y", &yWaveType, 1)) m_yWaveType = WaveType::Cosine;
        ImGui::SameLine();
        if (ImGui::RadioButton("Tangent##Y", &yWaveType, 2)) m_yWaveType = WaveType::Tangent;
        
        // Multipliers
        ImGui::SliderFloat("X Amplitude", &m_xMultiplier, 0.0f, 2.0f, "%.2f");
        ImGui::SliderFloat("Y Amplitude", &m_yMultiplier, 0.0f, 2.0f, "%.2f");
        ImGui::SliderFloat("Speed", &m_speedMultiplier, 0.1f, 5.0f, "%.2f");
    }
    
    // Scale settings
    ImGui::Separator();
    ImGui::Text("Scale Settings");
    ImGui::SliderFloat3("Scale", &m_scale.x, 0.1f, 2.0f);
   
    ImGui::Separator();
    ImGui::Text("Performance:");
    ImGui::Text("GPU Render Time: %.3f ms", m_lastRenderTimeMs);
    ImGui::Text("Est. FPS: %.1f", m_lastRenderTimeMs > 0 ? 1000.0f / m_lastRenderTimeMs : 0);
    

    ImGui::End();
}
} // namespace Mir