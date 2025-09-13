#pragma once
#include "glad/glad.h"
#include <vector>

namespace Mir {
    class EBO {
    public:
        EBO() { glGenBuffers(1, &m_EBO); }
        ~EBO() { glDeleteBuffers(1, &m_EBO); }
        void bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO); }
        void unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
        template<typename T>
        void setData(const std::vector<T>& data, GLenum usage = GL_STATIC_DRAW) {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(T), data.data(), usage);
        }
        GLuint id() const { return m_EBO; }
    private:
        GLuint m_EBO;
    };
}