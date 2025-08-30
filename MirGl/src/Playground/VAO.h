#pragma once
#include "glad/glad.h"
#include <vector>

namespace Mir {

    struct VertexAttribute {
        GLuint index;
        GLint size;
        GLenum type;
        GLboolean normalized;
        GLsizei stride;
        size_t offset;
    };

    using VertexLayouts = std::vector<VertexAttribute>;

    class VAO {
    public:
        VAO();
        ~VAO();

        void bind() const;
        void unbind() const;

        void setupVertexAttributes(const VertexLayouts& layout) const {
            for (const auto& attr : layout) {
                glVertexAttribPointer(attr.index, attr.size, attr.type, attr.normalized, attr.stride, (void*)attr.offset);
                glEnableVertexAttribArray(attr.index);
            }
        }

        GLuint id() const { return m_VAO; }

    private:
        GLuint m_VAO;
    };

}