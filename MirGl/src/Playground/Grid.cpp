#include "Grid.h"

#include "shader.h"

namespace Mir {

    Grid::Grid() {}

    void Grid::init() {
        float gridSize = 50.0f;
        std::vector<Vertex> gridVertices = {
            // position                          normal          color (unused for grid)
            {{-gridSize, -gridSize, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}},
            {{gridSize, -gridSize, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}},
            {{gridSize, gridSize, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}},
            {{-gridSize, gridSize, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}}};

        std::vector<unsigned int> gridIndices = {0, 1, 2, 2, 3, 0};

        m_VAO = std::make_unique<Mir::VAO>();
        m_VBO = std::make_unique<Mir::VBO>();
        m_EBO = std::make_unique<Mir::EBO>();

        m_VAO->bind();
        m_VBO->bind();
        m_VBO->setData(gridVertices);
        m_EBO->bind();
        m_EBO->setData(gridIndices);

        VertexLayouts gridLayout = {
            {0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, position)},
            {1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, normal)},
            {2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, color)}};
        m_VAO->setupVertexAttributes(gridLayout);

        // Create grid shader
        m_Shader = std::make_unique<Shader>("Playground_Grid.vs", "Playground_Grid.fs");
    }

    void Grid::draw(const glm::mat4& view, const glm::mat4& projection) {
        if (initalized) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            m_Shader->use();
            m_Shader->setMat4("view", view);
            m_Shader->setMat4("projection", projection);
            m_Shader->setMat4("model", glm::mat4(1.0f));
            m_Shader->setFloat("gridSize", 1.0f);

            // Draw grid quad
            m_VAO->bind();
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            glDisable(GL_BLEND);
        }
    }

    Grid::~Grid() {}
}  // namespace Mir