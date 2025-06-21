#pragma once
#include <imgui.h>

struct GLFWwindow;
namespace Mir
{
    struct RenderState;

    class ImGuiLayer
    {
    private:
        GLFWwindow* m_window;

    public:
        ImGuiLayer(GLFWwindow* window);
        ~ImGuiLayer();
        void render(RenderState& state);
        void begin();
        void End();
    };

} // namespace 
