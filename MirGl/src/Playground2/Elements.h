#pragma once

#include <math.h>

#include <glm/glm.hpp>
#include <variant>
#include <vector>

namespace Mir {
    namespace Playground2 {

        struct Vertex {
            glm::vec3 position;  // Position (x, y, z)
            glm::vec3 normal;    // Normal (nx, ny, nz)
            Vertex() = default;
            Vertex(float x, float y) : position(glm::vec3(x, y, 0)), normal(glm::vec3()) {}
            Vertex(const glm::vec3& pos, const glm::vec3& norm) : position(pos), normal(norm) {}
        };

        struct Color {
            unsigned char r;
            unsigned char g;
            unsigned char b;
            unsigned char a;
            Color(unsigned char r_, unsigned char g_, unsigned char b_, unsigned char a_)
                : r(r_), g(g_), b(b_), a(a_) {}
        };

        struct Circle {
            glm::vec3 pos;
            Color color;
            float radius;
            std::vector<Vertex> vertices;  // Unique vertex data for the circle

            Circle(const glm::vec3& p, float r, const Color& c) : pos(p), color(c), radius(r) {
                generateVertices();  // Generate vertex data on construction
            }

            void generateVertices(int segments = 32) {
                vertices.clear();
                vertices.emplace_back(
                    Vertex(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));  // Center vertex

                float theta = 2.0f * M_PI / segments;
                for (int i = 0; i <= segments; ++i) {
                    float x = std::cos(i * theta) * radius;
                    float y = std::sin(i * theta) * radius;
                    vertices.emplace_back(Vertex(glm::vec3(x, y, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));  // Edge vertices
                }
            }
        };

            struct Rectangle {
                float x;
                float y;
                float width;
                float height;
                std::vector<Vertex> vertices;

                Rectangle(float x_, float y_, float width_, float height_)
                    : x(x_), y(y_), width(width_), height(height_) {
                    generateVertices();
                }

                void generateVertices() {
                    vertices.clear();
                    vertices.emplace_back(Vertex(glm::vec3(x, y, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));  // Bottom-left
                    vertices.emplace_back(
                        Vertex(glm::vec3(x + width, y, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));  // Bottom-right
                    vertices.emplace_back(
                        Vertex(glm::vec3(x + width, y + height, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));  // Top-right
                    vertices.emplace_back(
                        Vertex(glm::vec3(x, y + height, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));  // Top-left
                }
            };

            using Element = std::variant<Circle, Rectangle>;
            using Elements = std::vector<Element>;

    }  // namespace Playground2

}  // namespace Mir