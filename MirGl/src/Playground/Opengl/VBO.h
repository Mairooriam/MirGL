#pragma once
#include <vector>

#include "glad/glad.h"


namespace Mir {
    class VBO {
      public:
        VBO();
        ~VBO();

        void bind() const;
        void unbind() const;

        // Generic data setting
        void setData(const void* data, size_t size, GLenum usage = GL_STATIC_DRAW);
        void updateData(size_t offset, const void* data, size_t size);

        // Template convenience methods
        template <typename T>
        void setData(const std::vector<T>& data, GLenum usage = GL_STATIC_DRAW) {
            setData(data.data(), data.size() * sizeof(T), usage);
        }

        template <typename T>
        void updateData(size_t offset, const T& item) {
            updateData(offset * sizeof(T), &item, sizeof(T));
        }

        GLuint getID() const { return m_VBO; }

      private:
        GLuint m_VBO;
    };
}  // namespace Mir