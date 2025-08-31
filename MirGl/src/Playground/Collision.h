#pragma once
#include <glm/glm.hpp>

namespace Mir {

    glm::vec2 worldToScreen(
        const glm::vec3& worldPos,
        const glm::mat4& model,
        const glm::mat4& view,
        const glm::mat4& projection,
        int windowWidth,
        int windowHeight) {
        glm::vec4 clipSpace = projection * view * model * glm::vec4(worldPos, 1.0f);
        glm::vec3 ndc = glm::vec3(clipSpace) / clipSpace.w;
        int screenX = int((ndc.x * 0.5f + 0.5f) * windowWidth);
        int screenY = int((1.0f - (ndc.y * 0.5f + 0.5f)) * windowHeight);
        return glm::vec2(screenX, screenY);
    }

    bool isPointSelected(
        const glm::vec3& point,
        const glm::mat4& model,
        const glm::mat4& view,
        const glm::mat4& projection,
        int windowWidth,
        int windowHeight,
        float mouseX,
        float mouseY,
        float selectionRadius) {
        glm::vec2 screenPos = worldToScreen(point, model, view, projection, windowWidth, windowHeight);
        float dist = glm::distance(glm::vec2(mouseX, mouseY), screenPos);
        return dist < selectionRadius;
    }
}  // namespace Mir