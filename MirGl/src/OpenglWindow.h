#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
namespace Mir
{


    class Window
    {
    private:
        GLFWwindow* m_window;  
    private:
        void processInput();
    public:
        Window(int w = 800, int h = 600, const char* name = "Learning OpenGL");
        void render();

        ~Window();
    };
    

} // namespace Mir
