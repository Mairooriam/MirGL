#pragma once
#include "Examples/IExample.h"

namespace Mir {

    class AnimatedTriangleExample : public IExample {
    public:
        AnimatedTriangleExample();
        ~AnimatedTriangleExample() override;
        
        void setup() override;
        void render() override;
        void cleanup() override;
        const char* getName() const override { return "Animated Triangle"; }

        void renderUI() override;
        
    private:
        std::unique_ptr<Shader> m_shader;
        unsigned int m_VAO, m_VBO;

        float m_animationSpeed = 1.0f;
        int m_animationType = 0;
        float m_animPhase = 0.0f; 
        glm::vec3 m_minColor = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 m_maxColor = glm::vec3(0.0f, 1.0f, 0.0f);

    };

} // namespace Mir