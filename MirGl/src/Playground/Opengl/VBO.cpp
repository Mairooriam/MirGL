#include "VBO.h"
namespace Mir {
    VBO::VBO() { 
        glGenBuffers(1, &m_VBO); 
    }
    
    VBO::~VBO() { 
        glDeleteBuffers(1, &m_VBO); 
    }
    
    void VBO::bind() const { 
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO); 
    }
    
    void VBO::unbind() const { 
        glBindBuffer(GL_ARRAY_BUFFER, 0); 
    }
    
    void VBO::setData(const void* data, size_t size, GLenum usage) {
        bind();
        glBufferData(GL_ARRAY_BUFFER, size, data, usage);
    }
    
    void VBO::updateData(size_t offset, const void* data, size_t size) {
        bind();
        glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
    }
}  // namespace Mir