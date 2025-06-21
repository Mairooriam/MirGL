#include "OpenglWindow.h"
#include "log.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
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
    //    float vertices[] = {
    //        -0.5f, -0.5f, 0.0f,
    //        0.5f, -0.5f, 0.0f,
    //        0.0f,  0.5f, 0.0f
    //    }; 
        float vertices[] = {
            0.5f,  0.5f, 0.0f,  // top right
            0.5f, -0.5f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f,  // bottom left
            -0.5f,  0.5f, 0.0f   // top left 
        };
        unsigned int indices[] = {  // note that we start from 0!
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
        };
        unsigned int EBO;
        glGenBuffers(1, &EBO);
        
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        unsigned int VBO;
        glGenBuffers(1, &VBO);  
        glBindBuffer(GL_ARRAY_BUFFER, VBO);  
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        
        const char *vertexShaderSource = "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "void main()\n"
            "{\n"
            "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
            "}\0";

        unsigned int vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        
        int  success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        
        if(!success)
        {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            MIR_ERROR("ERROR::SHADER::VERTEX::COMPILATION_FAILED {0}", infoLog);
        }


        const char *fragmentShaderSource = "#version 330 core\n"
            "out vec4 FragColor;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
            "}\0";

        unsigned int fragmentShader;
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            MIR_ERROR("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED {0}", infoLog);
        }

        unsigned int shaderProgram;
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            MIR_ERROR("ERROR::SHADER::PROGRAM::LINKING_FAILED {0}", infoLog);
        }

        glUseProgram(shaderProgram);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);  

        unsigned int VAO;
        glGenVertexArrays(1, &VAO);  
        glBindVertexArray(VAO);
        // 2. copy our vertices array in a vertex buffer for OpenGL to use
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // 3. copy our index array in a element buffer for OpenGL to use
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        // 4. then set the vertex attributes pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);  



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
            glUseProgram(shaderProgram);
            glBindVertexArray(VAO);
            //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glDrawArrays(GL_TRIANGLES, 0, 6);
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
