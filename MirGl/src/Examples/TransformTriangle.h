#pragma once
#include "Examples/IExample.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace Mir {



    class TransformTriangle : public IExample {
    public:
        TransformTriangle();
        ~TransformTriangle() override;
        
    
        void setup() override;
        void render() override;
        void cleanup() override;
        const char* getName() const override { return "Transform Triangle"; }

        void renderUI() override;
        
    private:
        std::unique_ptr<Shader> m_shader;
        unsigned int m_VAO, m_VBO, m_EBO;
        unsigned int m_texture1, m_texture2;

        float m_fade;
        glm::mat4 m_trans;
        float m_rotation;
        glm::vec3 m_scale{0.5f};
        glm::vec3 m_offset{0.3f};
        bool m_animate = false;
        enum class WaveType {
            Sine,
            Cosine,
            Tangent
        };
        GLuint m_queryID = 0;
        float m_lastRenderTimeMs = 0.0f;

        WaveType m_xWaveType = WaveType::Sine;
        WaveType m_yWaveType = WaveType::Cosine;
        float m_xMultiplier = 0.5f;
        float m_yMultiplier = 0.5f;
        float m_speedMultiplier = 1.0f;

    };

} // namespace Mir