#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
namespace Mir {

    struct Vertex {
        glm::vec3 position;                              // Position (x, y, z)
        glm::vec3 normal = glm::vec3(0.0f, 0.0f, 1.0f);  // Normal (nx, ny, nz)
        glm::vec3 color = glm::vec3(0.5f, 0.5f, 0.5f);
        bool selected = false;
        Vertex() = default;
        Vertex(float x, float y) : position(glm::vec3(x, y, 0)), normal(glm::vec3()) {}
        Vertex(const glm::vec3& pos) : position(pos) {}
        Vertex(const glm::vec3& pos, const glm::vec3& norm) : position(pos), normal(norm) {}
        Vertex(const glm::vec3& pos, const glm::vec3& norm, const glm::vec3& col)
            : position(pos), normal(norm), color(col) {}
    };

    enum class DrawMode {
        Triangles,
        TriangleFan,
        Lines,
        Points,
        IndexedTriangles,
        // Add more as needed
    };

    class Object {
      public:
        int id;
        glm::vec3 color;
        glm::mat4 modelMatrix;
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<bool> selectedFaces;
        DrawMode drawMode = DrawMode::Triangles;
        bool isSelected = false;

        Object(const std::vector<Vertex>& verts)
            : id(nextId++), color(glm::vec3(1.0f)), modelMatrix(glm::mat4(1.0f)), vertices(verts), isSelected(false) {
            updateSelectedFaces();
        }

        void setPosition(const glm::vec3& position) { modelMatrix = glm::translate(glm::mat4(1.0f), position); }

        void setScale(const glm::vec3& scale) { modelMatrix = glm::scale(modelMatrix, scale); }

        void setRotation(float angle, const glm::vec3& axis) {
            modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), axis);
        }
        void setColor(const glm::vec3& col) { color = col; }

        const std::vector<unsigned int>& getIndices() const { return indices; }
        void setIndices(const std::vector<unsigned int>& idx) { indices = idx; }

        void setDrawMode(DrawMode mode) { drawMode = mode; }

        void updateSelectedFaces() {
            if (drawMode == DrawMode::IndexedTriangles) {
                selectedFaces.resize(indices.size() / 3, false);
            } else if (drawMode == DrawMode::TriangleFan) {
                selectedFaces.resize(vertices.size() - 2, false);
            } else {
                selectedFaces.resize(vertices.size() / 3, false);
            }
        }
        virtual void update(float /*deltaTime*/ = 0.0f) {}

      private:
        static int nextId;
    };

    enum class OriginLocation { BOTTOM_LEFT, BOTTOM_RIGHT, TOP_LEFT, TOP_RIGHT, CENTER };

    class Square : public Object {
      public:
        Square(
            const glm::vec3& position = glm::vec3(0.0f),
            float size = 1.0f,
            OriginLocation origin = OriginLocation::CENTER)
            : Object(createSquareVertices(size, origin)) {
            setPosition(position);
            setIndices({0, 1, 2, 2, 3, 0});
            setDrawMode(DrawMode::IndexedTriangles);
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
                {{-halfSize + originOffset.x, -halfSize + originOffset.y, 0.0f}, {0.0f, 0.0f, 1.0f}},  // 0: Bottom-left
                {{halfSize + originOffset.x, -halfSize + originOffset.y, 0.0f}, {0.0f, 0.0f, 1.0f}},  // 1: Bottom-right
                {{halfSize + originOffset.x, halfSize + originOffset.y, 0.0f}, {0.0f, 0.0f, 1.0f}},   // 2: Top-right
                {{-halfSize + originOffset.x, halfSize + originOffset.y, 0.0f}, {0.0f, 0.0f, 1.0f}}   // 3: Top-left
            };
        }
    };

    class Circle : public Object {
      public:
        Circle(float radius, int segments, glm::vec3 color = glm::vec3(1.0f));

      private:
        std::vector<Vertex> createCircleVertices(float radius, int segments);
    };

    class Dot {
      private:
        /* data */
      public:
        Dot(/* args */);
        ~Dot();
    };

    class Line : public Object {
      private:
      public:
        Line(const glm::vec3& startPoint, const glm::vec3& endPoint);
        ~Line();
    };

    class BezierCurve : public Object {
      private:
        std::vector<Vertex> createCurveVertices(int segments_);
				void updateCurve();

      public:
        glm::vec3 p0, cp1, cp2, p1;
        BezierCurve(const glm::vec3 p0_, const glm::vec3& cp1_, const glm::vec3& cp2_, const glm::vec3& p2_);
        ~BezierCurve();
				void update(float dt) override { updateCurve(); };
    };

    struct DragDrop {
        int objectIdx = -1;
        int vertexIdx = -1;
        bool isDraggingVertex = false;
        int globalVboOffset = -1;

        Object* obj = nullptr;
    };

}  // namespace Mir