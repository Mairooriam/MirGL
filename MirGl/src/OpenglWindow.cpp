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
#include "Examples/CoordinateSystems.h"
#include "Examples/CameraSystem.h"

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
        glfwSwapInterval(1); // Enable VSync
        
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
        m_renderState.exampleManager->addExample(std::make_unique<CoordinateSystems>());
        m_renderState.exampleManager->addExample(std::make_unique<CameraSystem>());
        m_renderState.exampleManager->selectExample(5);
    }
    


    
void Window::render() {
    double lastTime = glfwGetTime();
    
    while (!glfwWindowShouldClose(m_window)) {
        double currentTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentTime - lastTime);
        lastTime = currentTime;
        
        // More accurate FPS calculation
        m_frameCount++;
        m_frameTimeAccum += deltaTime;
        
        // Update every 60 frames or every 0.5 seconds, whichever comes first
        if (m_frameCount >= 60 || m_frameTimeAccum >= 0.5f) {
            m_fps = m_frameCount / m_frameTimeAccum;
            m_frameTime = m_frameTimeAccum / m_frameCount;
            m_frameTimeAccum = 0;
            m_frameCount = 0;
        }


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
        // for now here. lazy
        int windowX, windowY;
        glfwGetWindowPos(m_window, &windowX, &windowY);

        ImGui::SetNextWindowPos(ImVec2(windowX + 10, windowY + 10), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
        ImGui::Begin("FPS Counter", nullptr, 
                    ImGuiWindowFlags_NoTitleBar | 
                    ImGuiWindowFlags_NoResize | 
                    ImGuiWindowFlags_AlwaysAutoResize | 
                    ImGuiWindowFlags_NoMove | 
                    ImGuiWindowFlags_NoSavedSettings);

        ImGui::Text("FPS: %.1f (%.2f ms/frame)", m_fps, m_frameTime * 1000.0f);
        if (ImGui::Button("Toggle VSync")) {
            static bool vsyncEnabled = true;
            vsyncEnabled = !vsyncEnabled;
            glfwSwapInterval(vsyncEnabled ? 1 : 0);
        }
        ImGui::End();


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
