#pragma once
#include <glm/glm.hpp>
namespace Mir{
    namespace Utils{
        glm::vec2 fromViewToWorld(const glm::vec2& pos){

        }

        glm::vec2 fromWorldToView(const glm::vec2& pos, const glm::mat4& viewMatrix) {
            glm::vec4 worldPos = glm::vec4(pos, 0.0f, 1.0f); // Convert 2D position to 4D (homogeneous coordinates)
            glm::vec4 viewPos = viewMatrix * worldPos;       // Transform using the view matrix
            return glm::vec2(viewPos.x, viewPos.y);          // Return the 2D view-space position
        }

    }
}