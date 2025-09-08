#pragma once

#include "shader.h"
#include "VBO.h"
#include "VAO.h"
#include "Primitives.h"
#include "EBO.h"
namespace Mir{

    class Grid
    {
    private:
        std::unique_ptr<Shader> m_Shader;
        std::unique_ptr<VBO> m_VBO;
        std::unique_ptr<VAO> m_VAO;
        std::unique_ptr<EBO> m_EBO;
        bool initalized = false;
    public:
        Grid();
        void init();
        void draw(const glm::mat4& view, const glm::mat4& projection);
        ~Grid();
    };

    
}