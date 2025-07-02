#include "OpenglWindow.h"
#include "log.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <iostream>
#include "shader.h"

#include "Examples/AnimatedTriangle.h"
#include "Examples/ColorTriangle.h"
#include "Examples/TexturedTriangle.h"
#include "Examples/TransformTriangle.h"

namespace Mir {
    void framebuffer_size_callback(GLFWwindow* /*window*/, int width, int height) {
        glViewport(0, 0, width, height);
    }

    void Window::processInput() {
        if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
            m_imguiLayer->terminate();
            glfwSetWindowShouldClose(m_window, true);

        }

    }


    static void glfw_error_callback(int error, const char* description)
    {
        fprintf(stderr, "GLFW Error %d: %s\n", error, description);
    }


    Window::Window(int w_, int h_, const char* name_) {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
        glfwSetErrorCallback(glfw_error_callback);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

        m_window = glfwCreateWindow(w_, h_, name_, nullptr, nullptr);
        if (m_window == nullptr) {
            MIR_ERROR("Failed to create GLFW window");
            glfwTerminate();
            return;
        }
        glfwMakeContextCurrent(m_window);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            MIR_ERROR("Failed to initialize GLAD");

        glViewport(0, 0, 800, 600);
        
        glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
        
        m_imguiLayer = std::make_unique<ImGuiLayer>(m_window);
        m_exampleManager = ExampleManager();

        m_renderState.exampleManager = &m_exampleManager;
        m_renderState.exampleManager->addExample(std::make_unique<ColorTriangleExample>());
        m_renderState.exampleManager->addExample(std::make_unique<AnimatedTriangleExample>());
        m_renderState.exampleManager->addExample(std::make_unique<TexturedTriangle>());
        m_renderState.exampleManager->addExample(std::make_unique<TransformTriangle>());
        m_renderState.exampleManager->selectExample(3);
    }
    


    
void Window::render() {
    while (!glfwWindowShouldClose(m_window)) {
        processInput();
        glfwPollEvents();
        
        if (glfwGetCurrentContext() != m_window) {
            glfwMakeContextCurrent(m_window);
            // Optionally, you can check if the context is set correctly after calling
            if (glfwGetCurrentContext() != m_window) {
                MIR_ERROR("Failed to make context current, skipping frame");
                continue;
            }
        }
        // Clear framebuffer with UI-controlled color
        glClearColor(
            m_renderState.clearColor[0], 
            m_renderState.clearColor[1], 
            m_renderState.clearColor[2], 
            m_renderState.clearColor[3]
        );
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Draw your OpenGL content with UI-controlled settings
        glPolygonMode(GL_FRONT_AND_BACK, m_renderState.polygonMode);
        
        if (m_exampleManager.currentExample) {
            m_exampleManager.currentExample->render();
        }
        
        m_imguiLayer->begin();
        //renderExamplesUI(); // New method to render example selector
        m_imguiLayer->render(m_renderState);
        m_imguiLayer->End();
        
        // Swap buffers
        glfwSwapBuffers(m_window);
    }
}

    void Window::addExample(std::unique_ptr<IExample> example) {
        m_exampleManager.addExample(std::move(example));
    }

    void Window::selectExample(size_t index) {
        m_exampleManager.selectExample(index);
    }

    Window::~Window() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        
        glfwTerminate();
    }

}  // namespace Mir
