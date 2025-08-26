#include "PickingTexture.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
namespace Mir{

    PickingTexture::PickingTexture() {}
    
    PickingTexture::~PickingTexture() {}
    
    void PickingTexture::Init(unsigned int WindowWidth, unsigned int WindowHeight) {
        // Example: Generate a texture for picking
        glGenFramebuffers(1, &m_fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

        glGenTextures(1, &m_pickingTexture);
        glBindTexture(GL_TEXTURE_2D, m_pickingTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32UI, WindowWidth, WindowHeight, 0, GL_RGB_INTEGER, GL_UNSIGNED_INT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pickingTexture, 0);
        // Depth texture
        glGenTextures(1, &m_depthTexture);
        glBindTexture(GL_TEXTURE_2D, m_depthTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

        if (status != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "Framebuffer not complete!" << std::endl;
            std::cout << status << std::endl;
        }

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);


    }
    
    void PickingTexture::EnableWriting() {
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
        GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
        glDrawBuffers(1, DrawBuffers);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    
    void PickingTexture::DisableWriting() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    PickingTexture::PixelInfo PickingTexture::ReadPixel(unsigned int x, unsigned int y) {
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
        PixelInfo pixelInfo;
        glReadPixels(x, y, 1, 1, GL_RGB_INTEGER, GL_UNSIGNED_INT, &pixelInfo);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return pixelInfo;
    }
}
