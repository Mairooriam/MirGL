#pragma once
#include "Examples/IExample.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace Mir {



    class CoordinateSystems : public IExample {
    public:
        CoordinateSystems();
        ~CoordinateSystems() override;
        
    
        void setup() override;
        void render() override;
        void cleanup() override;
        const char* getName() const override { return "Coordinate Systems"; }

        void renderUI() override;
        
    private:
        std::unique_ptr<Shader> m_shader;
        unsigned int m_VAO, m_VBO, m_EBO;
        unsigned int m_texture1, m_texture2;

        std::vector<glm::vec3> m_cubePositions;

        float m_fade;
        glm::mat4 m_trans;
        float m_rotation;
        glm::vec3 m_scale{0.5f};
        glm::vec3 m_offset{0.3f};



        GLuint m_queryID = 0;
        float m_lastRenderTimeMs = 0.0f;

        enum class renderMode {
            SingleBox,
            MultipleBoxes
        };
        renderMode m_rMode = renderMode::MultipleBoxes;
        float m_FOV = 45.0f;
        float m_aspectRatio = 2.0f;
        double m_time = 0.0f;
        float m_delay = 1.0f;
        double m_lastFrameTime = 0.0f;
        double m_deltaTime = 0.0f;
         

    };

} // namespace Mir