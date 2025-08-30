#include "VBO.h"
namespace Mir {
    VBO::VBO() { glGenBuffers(1, &m_VBO); }
    VBO::~VBO() { glDeleteBuffers(1, &m_VBO); }
    void VBO::bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_VBO); }
    void VBO::unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }
}