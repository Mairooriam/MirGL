#pragma once

#include "EBO.h"
#include "Primitives.h"
#include "VAO.h"
#include "VBO.h"
#include "shader.h"

namespace Mir {

    class Grid {
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
        struct Config {
            float size = 5.0f;
            glm::vec4 color = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
            float lineWidth = 1.0f;
            float fadeDistance = 0.0f;
            float fadeStrength = 0.4f;
            bool enableFading = true;
        } config;

        ~Grid();
    };

}  // namespace Mir