#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Elements.h"

namespace Mir {
    namespace Playground2 {

        struct Draw {
            Shader& shaderProgram;  // Shader program to use for rendering
            size_t vertexOffset;   // Offset for vertex drawing
            const glm::mat4& view; // View matrix
            const glm::mat4& projection; // Projection matrix

            Draw(Shader& shader, size_t offset, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
                : shaderProgram(shader), vertexOffset(offset), view(viewMatrix), projection(projectionMatrix) {}

            void operator()(const Circle& c) {
                std::cout << "Rendering Circle. Offset" << vertexOffset << "\n";

                // Set the model matrix for the circle
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, c.pos);

                glEnable(GL_DEPTH_TEST);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                shaderProgram.use();
                shaderProgram.setMat4("model", model);
                shaderProgram.setMat4("view", view);
                shaderProgram.setMat4("projection", projection);
                shaderProgram.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
                //shaderProgram.setVec3("lightPos", m_lightPosition);

                // Draw the circle
                glDrawArrays(GL_TRIANGLE_FAN, vertexOffset, c.vertices.size());
                vertexOffset += c.vertices.size();  // Update the vertex offset
            }

            void operator()(const Rectangle& r) {
                std::cout << "Rendering Rectangle. offset" << vertexOffset <<"\n";
                // Draw the rectangle using raylib's DrawRectangleRec
                //::DrawRectangleRec(ToRaylib(r), ::WHITE); // Default color: white
            }
        };

    }  // namespace Playground2
}  // namespace Mir