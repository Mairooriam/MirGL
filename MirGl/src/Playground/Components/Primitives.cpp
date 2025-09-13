#include "Primitives.h"
constexpr double PI = 3.141592653589793;

namespace Mir {

    int Object::nextId = 1;

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

    Line::Line(const glm::vec3& startPoint, const glm::vec3& endPoint)
        : Object({Vertex(startPoint), Vertex(endPoint)}) {
        setDrawMode(DrawMode::Points);
    }

    Line::~Line() {}

    BezierCurve::BezierCurve(const glm::vec3 p0_, const glm::vec3& cp1_, const glm::vec3& cp2_, const glm::vec3& p1_)
        : p0(p0_), cp1(cp1_), cp2(cp2_), p1(p1_), Object({}) {
        vertices = createCurveVertices(32);  // Now uses correct control points
        setDrawMode(DrawMode::Points);
    }
    std::vector<Vertex> BezierCurve::createCurveVertices(int segments_) {
        std::vector<Vertex> curve;
        curve.push_back(p0);
        curve.push_back(cp1);
        curve.push_back(cp2);
        curve.push_back(p1);
        for (int i = 0; i <= segments_; ++i) {
            float t = float(i) / segments_;
            // Cubic Bézier formula
            glm::vec3 point = (1 - t) * (1 - t) * (1 - t) * p0 + 3 * (1 - t) * (1 - t) * t * cp1 +
                3 * (1 - t) * t * t * cp2 + t * t * t * p1;
            curve.emplace_back(Vertex(point, glm::vec3(0.0f, 0.0f, 1.0f)));
        }
        return curve;
    }
    BezierCurve::~BezierCurve() {}

    void BezierCurve::updateCurve() {
        vertices = createCurveVertices(32);
    }

}  // namespace Mir
