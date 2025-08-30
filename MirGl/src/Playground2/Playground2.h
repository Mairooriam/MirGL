#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "Camera.h"
#include "Elements.h"
#include "Examples/IExample.h"
#include "Mouse.h"
#include "PlayGroundCamera.h"
#include "shader.h"


namespace Mir {
    namespace Playground2 {
        struct DebugData {
            Mouse mouse;
        };

        class Playground2 : public IExample {
          private:
          private:
            enum class ControlState {
                PERSPECTIVE_CAMERA,
                ORTHO_CAMERA,
            } controlState_m;

            std::vector<std::shared_ptr<Camera>> cameras_m;
                
        
            size_t selectedCamera = 0;
            Camera* getCurrentCamera() { return cameras_m[selectedCamera].get(); }

            void switchCamera(ControlState state) {
                controlState_m = state;
                selectedCamera = (state == ControlState::ORTHO_CAMERA) ? 1 : 0;
            }

          private:
            std::unordered_map<std::string, std::unique_ptr<Shader>> shaders_m;
            Elements objects_m;
            Mouse mouse_m;
            double m_lastFrameTime = 0.0f;
            double m_deltaTime = 0.0f;
            unsigned int m_VAO, m_VBO;

          public:
            Playground2(/* args */);

            ~Playground2() override;
            void setup() override;
            void render() override;
            void cleanup() override;
            const char* getName() const override { return "Playground2"; }
            void renderUI() override;
            void handleInput();
        };

    }  // namespace Playground2

}  // namespace Mir