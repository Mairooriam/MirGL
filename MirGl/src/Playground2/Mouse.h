#pragma once
#include <glm/glm.hpp>
namespace Mir{
    namespace Playground2 {
        struct Mouse {
            bool isClicked = false;
            glm::vec2 screen_pos = glm::vec2(0.0f,0.0f);
            glm::vec2 view_pos = glm::vec2(0.0f,0.0f);
            glm::vec2 world_pos = glm::vec2(0.0f,0.0f);
        };

    }

}