#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <vector>

#include "Camera.h"
#include "DebugData.h"
#include "DebugUI.h"
#include "Examples/IExample.h"
#include "Mouse.h"
#include "Primitives.h"
#include "VAO.h"
#include "VBO.h"
#include "Light.h"

namespace Mir {

  
  enum class ActiveWindow { MAIN_VIEWPORT, SECOND_VIEWPORT };
    class Playground : public IExample {
      public:
        Playground();
        void updateDebugData();
        DebugData dData_m;

        void UpdateScrollOffsets(float xOffset, float yOffset) {
            m_scrollXOffset = xOffset;
            m_scrollYOffset = yOffset;
        };


        void handleInput();
        OrthoCamera* getOrthoCamera() { return m_orthoCamera.get(); }
        Camera* getCamera() { return m_Camera.get(); }

        ~Playground() override;
        void setup() override;
        void render() override;
        void drawObjects(const glm::mat4& view, const glm::mat4& projection);
        void drawLights(const glm::mat4& view, const glm::mat4& projection);
        void cleanup() override;
        void updateTime();
        const char* getName() const override { return "Playground"; }
        void renderUI() override;

        glm::vec3 ScreenToWorld(float x_screen, float y_screen);
        bool useOrthoCamera = true;

      private:
        DebugUI dbUI_m;

        ActiveWindow m_activeWindow = ActiveWindow::SECOND_VIEWPORT;

        std::vector<Object> objects_m;
        std::vector<Light> lights_m;

        // CAMERA
        std::unique_ptr<Camera> m_Camera;
        std::unique_ptr<OrthoCamera> m_orthoCamera;
        bool showWireFrame = false;
        float m_FOV = 45.0f;
        int m_windowWidth = 800;
        int m_windowHeight = 600;
        float m_aspectRatio = static_cast<float>(m_windowWidth) / static_cast<float>(m_windowHeight);
        
        // VBO & VAO
        std::unique_ptr<Mir::VAO> m_VAO;
        std::unique_ptr<Mir::VBO> m_VBO;
        std::unique_ptr<Mir::VAO> m_lightVAO;
        unsigned int m_texture1, m_texture2;
        
        // SHADER
        std::unique_ptr<Shader> m_shader;
        std::unique_ptr<Shader> m_lightingShader;
        
        // MOUSE
        Mouse mouse_m;
        double m_scrollXOffset = 0.0;  // Horizontal scroll offset
        double m_scrollYOffset = 0.0;  // Vertical scroll offset
        bool m_updateMousePos = true;
        glm::vec2 m_manualMousePos = glm::vec2(0.0f, 0.0f);

        //TIME
        double m_time = 0.0f;
        float m_delay = 1.0f;
        double m_lastFrameTime = 0.0f;
        double m_deltaTime = 0.0f;


        GLuint m_queryID = 0;
        float m_lastRenderTimeMs = 0.0f;

        // for second camera window
        GLuint m_perspectiveFramebuffer = 0;
        GLuint m_perspectiveTextureColorbuffer = 0;
        GLuint m_perspectiveRBO = 0;



        float m_rotationAngle = 50.0f;
        glm::vec3 m_rotationAxis = glm::vec3(0.0f, 0.0f, 1.0f);
        glm::vec3 m_lightPosition = glm::vec3(0.0f, 0.0f, 2.5f);

    };

}  // namespace Mir