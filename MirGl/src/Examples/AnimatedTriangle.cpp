#include "Examples/AnimatedTriangle.h"
#include "log.h"

namespace Mir {

AnimatedTriangleExample::AnimatedTriangleExample() = default;

AnimatedTriangleExample::~AnimatedTriangleExample() {
    cleanup();
}

void AnimatedTriangleExample::setup() {
    m_shader = std::make_unique<Shader>("AnimatedTriangle.vs", "AnimatedTriangle.fs");
    
    float vertices[] = {
        // positions         
         0.5f, -0.5f, 0.0f,    // bottom right
        -0.5f, -0.5f, 0.0f,    // bottom left
         0.0f,  0.5f, 0.0f,    // top 
    };
    
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    
    glBindVertexArray(m_VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void AnimatedTriangleExample::render() {
    float timeValue = glfwGetTime();
    float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
    
    m_shader->use();
    m_shader->setVec4("ourColor", glm::vec4(0.0f, greenValue, 0.0f, 1.0f));
    
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

void AnimatedTriangleExample::cleanup() {
    if (m_VAO) {
        glDeleteVertexArrays(1, &m_VAO);
        m_VAO = 0;
    }
    if (m_VBO) {
        glDeleteBuffers(1, &m_VBO);
        m_VBO = 0;
    }
    m_shader.reset();
}

} // namespace Mir