#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
namespace Mir {

    struct Vertex {
        glm::vec3 position;  // Position (x, y, z)
        glm::vec3 normal;    // Normal (nx, ny, nz)
    };

    class Object {
      public:
        std::vector<Vertex> vertices;
        glm::mat4 modelMatrix;

        Object(const std::vector<Vertex>& verts) : vertices(verts), modelMatrix(glm::mat4(1.0f)) {}

        void setPosition(const glm::vec3& position) { modelMatrix = glm::translate(glm::mat4(1.0f), position); }

        void setScale(const glm::vec3& scale) { modelMatrix = glm::scale(modelMatrix, scale); }

        void setRotation(float angle, const glm::vec3& axis) {
            modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), axis);
        }
    };

    enum class OriginLocation { BOTTOM_LEFT, BOTTOM_RIGHT, TOP_LEFT, TOP_RIGHT, CENTER };

    class Square : public Object {
      public:
        Square(
            const glm::vec3& position = glm::vec3(0.0f), float size = 1.0f,
            OriginLocation origin = OriginLocation::CENTER)
            : Object(createSquareVertices(size, origin)) {
            setPosition(position);
        }

      private:
        static std::vector<Vertex> createSquareVertices(float size, OriginLocation origin) {
            float halfSize = size / 2.0f;
            // Adjust the origin offset
            glm::vec3 originOffset(0.0f);
            switch (origin) {
                case OriginLocation::BOTTOM_LEFT: originOffset = glm::vec3(halfSize, halfSize, 0.0f); break;
                case OriginLocation::BOTTOM_RIGHT: originOffset = glm::vec3(-halfSize, halfSize, 0.0f); break;
                case OriginLocation::TOP_LEFT: originOffset = glm::vec3(halfSize, -halfSize, 0.0f); break;
                case OriginLocation::TOP_RIGHT: originOffset = glm::vec3(-halfSize, -halfSize, 0.0f); break;
                case OriginLocation::CENTER:
                default: originOffset = glm::vec3(0.0f, 0.0f, 0.0f); break;
            }

            // Create vertices with the adjusted origin
            return {
                {{-halfSize + originOffset.x, -halfSize + originOffset.y, 0.0f}, {0.0f, 0.0f, 1.0f}},  // Bottom-left
                {{halfSize + originOffset.x, -halfSize + originOffset.y, 0.0f}, {0.0f, 0.0f, 1.0f}},   // Bottom-right
                {{halfSize + originOffset.x, halfSize + originOffset.y, 0.0f}, {0.0f, 0.0f, 1.0f}},    // Top-right
                {{halfSize + originOffset.x, halfSize + originOffset.y, 0.0f}, {0.0f, 0.0f, 1.0f}},    // Top-right
                {{-halfSize + originOffset.x, halfSize + originOffset.y, 0.0f}, {0.0f, 0.0f, 1.0f}},   // Top-left
                {{-halfSize + originOffset.x, -halfSize + originOffset.y, 0.0f}, {0.0f, 0.0f, 1.0f}}   // Bottom-left
            };
        }
    };
}  // namespace Mir