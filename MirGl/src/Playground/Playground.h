#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "Camera.h"
#include "Examples/IExample.h"
#include "Primitives.h"

namespace Mir {

    struct Mouse {
        double x_screen;
        double y_screen;
        double x_view;
        double y_view;
        double x_world;
        double y_world;
    };

    enum class ActiveWindow { MAIN_VIEWPORT, SECOND_VIEWPORT };

    struct DebugData {
        glm::vec2 ndc;                // Normalized Device Coordinates (NDC)
        glm::mat4 inverseProjection;  // Inverse Projection Matrix
        glm::mat4 inverseView;        // Inverse View Matrix
        glm::vec3 worldPosition;      // World Position
    };

    class Playground : public IExample {
      public:
        Playground();
        ~Playground() override;
        void debugUI_CameraInfo();
        void debugUI_screenToWorld();
        void debugUI_windowInfo();


        void setup() override;
        void setupFrameBuffers();
        void renderToFrameBuffer();
        void handleInput();
        void render() override;
        void cleanup() override;
        const char* getName() const override { return "Playground"; }

        glm::vec3 ScreenToWorld(float x_screen, float y_screen);
        void renderUI() override;

      private:
        ActiveWindow m_activeWindow = ActiveWindow::MAIN_VIEWPORT;

        std::vector<Object> objects_m;
        std::vector<Vertex> objectsVertexData_m;
        std::unique_ptr<Shader> m_shader;
        std::unique_ptr<Shader> m_lightingShader;
        std::unique_ptr<Camera> m_Camera;
        std::unique_ptr<OrthoCamera> m_orthoCamera;
        unsigned int m_VAO, m_VBO, m_EBO, m_lightVAO;
        unsigned int m_texture1, m_texture2;

        Mouse mouse_m;
        bool useOrthoCamera = true;

        GLuint m_queryID = 0;
        float m_lastRenderTimeMs = 0.0f;

        // for second camera window
        GLuint m_perspectiveFramebuffer = 0;
        GLuint m_perspectiveTextureColorbuffer = 0;
        GLuint m_perspectiveRBO = 0;
      
        DebugData debugData_m;
        bool m_updateMousePos = true;
        glm::vec2 m_manualMousePos = glm::vec2(0.0f,0.0f);

        float m_FOV = 45.0f;
        int m_windowWidth = 800;
        int m_windowHeight = 600;
        float m_aspectRatio = static_cast<float>(m_windowWidth) / static_cast<float>(m_windowHeight);
        double m_time = 0.0f;
        float m_delay = 1.0f;
        double m_lastFrameTime = 0.0f;
        double m_deltaTime = 0.0f;
    };

}  // namespace Mir