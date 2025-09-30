module;
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
export module nuit:imgui;

namespace Nuit
{
    export void init_imgui(GLFWwindow* window, const char* openglVersion = "#version 330")
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(openglVersion);
    }

    export void begin_imgui_frame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
    	ImGui::NewFrame();
    	// ImGui::DockSpaceOverViewport(ImGui::GetMainViewport()->ID);
    }

    export void render_imgui()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    export void shutdown_imgui()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    export void debug_window()
    {
        ImGuiIO& io = ImGui::GetIO();
        ImGui::Begin("Debug Information");
        ImGui::Text("FPS: %.1f", io.Framerate);
        ImGui::Text("Mouse Pos: (%.1f, %.1f)", io.MousePos.x, io.MousePos.y);
        ImGui::End();
    }
} // namespace Nuit
