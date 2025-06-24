#pragma once
#include "Examples/IExample.h"

namespace Mir {



    class TexturedTriangle : public IExample {
    public:
        TexturedTriangle();
        ~TexturedTriangle() override;
        
        void setup() override;
        void render() override;
        void cleanup() override;
        const char* getName() const override { return "Textured Triangle"; }

        void renderUI() override;
        
    private:
        std::unique_ptr<Shader> m_shader;
        unsigned int m_VAO, m_VBO, m_EBO;
        unsigned int m_texture1, m_texture2;

        float m_fade;
    };

} // namespace Mir