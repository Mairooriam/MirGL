#include "Primitives.h"
constexpr double PI = 3.141592653589793;

namespace Mir {

    unsigned int Object::nextId = 1;

    Circle::Circle(float radius, int segments, glm::vec3 Color) : Object(createCircleVertices(radius, segments)) {
        setPosition(glm::vec3(0.0f));
        setColor(Color);
        drawMode = DrawMode::TriangleFan;
    }

    std::vector<Vertex> Circle::createCircleVertices(float radius, int segments) {
        float theta = PI * 2 / segments;
        std::vector<Vertex> res;

        res.emplace_back(Vertex(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));

        for (int i = 0; i < segments; i++) {
            float x = glm::cos(theta * i) * radius;
            float y = glm::sin(theta * i) * radius;
            res.emplace_back(Vertex(glm::vec3(x, y, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
        }

        res.emplace_back(res[1]);

        return res;
    }

}  // namespace Mir
