#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "ImGuiLayer.h"
#include <memory>
namespace Mir
{
    struct RenderState {
        GLenum polygonMode = GL_LINE;
        bool showWireframe = true;
        float clearColor[4] = {0.2f, 0.3f, 0.3f, 1.0f};
    };

    class Window
    {
    private:
        GLFWwindow* m_window; 
        std::unique_ptr<ImGuiLayer> m_imguiLayer;
        RenderState m_renderState;
    private:
        void processInput();
    public:
        Window(int w = 800, int h = 600, const char* name = "Learning OpenGL");
        void render();

        ~Window();
    };
    

} // namespace Mir
