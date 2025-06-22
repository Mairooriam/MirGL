#include "OpenglWindow.h"
#include "log.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#include <iostream>
#include "shader.h"
namespace Mir {
    void framebuffer_size_callback(GLFWwindow* /*window*/, int width, int height) {
        glViewport(0, 0, width, height);
    }

    void Window::processInput() {
        if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(m_window, true);
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
        }
        glfwMakeContextCurrent(m_window);
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            MIR_ERROR("Failed to initialize GLAD");

        glViewport(0, 0, 800, 600);
        
        glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
        
        m_imguiLayer = std::make_unique<ImGuiLayer>(m_window);
    }
    


    
    void Window::render() {

        float firstTriangle[] = {
            -0.9f, -0.5f, 0.0f,  // left 
            -0.0f, -0.5f, 0.0f,  // right
            -0.45f, 0.5f, 0.0f,  // top 
        };
        float secondTriangle[] = {
            0.0f, -0.5f, 0.0f,  // left
            0.9f, -0.5f, 0.0f,  // right
            0.45f, 0.5f, 0.0f   // top 
        };
        unsigned int indices[] = {
            0, 1, 2,  // first triangle
            1, 4, 5,  // third triangle (using vertices from both triangles)
            3, 4, 5   // second triangle
        };
        

        unsigned int VBOs[2], VAOs[2];
        glGenVertexArrays(2, VAOs); // we can also generate multiple VAOs or buffers at the same time
        glGenBuffers(2, VBOs);
        // first triangle setup
        // --------------------
        glBindVertexArray(VAOs[0]);
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// Vertex attributes stay the same
        glEnableVertexAttribArray(0);
        // glBindVertexArray(0); // no need to unbind at all as we directly bind a different VAO the next few lines
        // second triangle setup
        // ---------------------
        glBindVertexArray(VAOs[1]);	// note that we bind to a different VAO now
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);	// and a different VBO
        glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // because the vertex data is tightly packed we can also specify 0 as the vertex attribute's stride to let OpenGL figure it out
        glEnableVertexAttribArray(0);
        // glBindVertexArray(0); // not really necessary as well, but beware of calls that could affect VAOs while this one is bound (like binding element buffer objects, or enabling/disabling vertex attributes)

        
        int nrAttributes;
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
        std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;


        Shader shader("shader.fs", "shader.vs");
        while (!glfwWindowShouldClose(m_window)) {
            processInput();
            glfwPollEvents();
            // Make main OpenGL context current
            glfwMakeContextCurrent(m_window);
            
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
            shader.use();
            glBindVertexArray(VAOs[0]);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            glBindVertexArray(VAOs[1]);
            glDrawArrays(GL_TRIANGLES, 0, 3);
            glBindVertexArray(0);
            
            m_imguiLayer->begin();
            m_imguiLayer->render(m_renderState);
            m_imguiLayer->End();
            // Swap buffers
            glfwSwapBuffers(m_window);
        }
    }

    Window::~Window() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        
        glfwTerminate();
    }

}  // namespace Mir
