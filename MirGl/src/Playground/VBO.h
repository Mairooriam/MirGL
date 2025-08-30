#pragma once
#include "glad/glad.h"
#include <vector>
namespace Mir {
    class VBO {
    public:
        VBO();
        ~VBO();

        void bind() const;
        void unbind() const;

        // Upload data to the buffer
        template<typename T>
        void setData(const std::vector<T>& data, GLenum usage = GL_STATIC_DRAW) {
            glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
            glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(T), data.data(), usage);
        }

        GLuint id() const { return m_VBO; }

    private:
        GLuint m_VBO;
    };
}