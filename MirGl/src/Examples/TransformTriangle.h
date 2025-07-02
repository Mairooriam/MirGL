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
    };

} // namespace Mir