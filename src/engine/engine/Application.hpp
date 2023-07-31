#pragma once

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include <iostream>
#include <filesystem>
#include <memory>
#include <vector>

#include "pcg/PCGHelpers.hpp"

#include "guitools/AppGUI.hpp"

#include "scenes/Scene.hpp"

namespace lei3d
{
    class AppGUI;
    class Scene;

    class Application
    {
    public:
        Application();
        ~Application();

        void Run(); // Run the app.

        GLFWwindow* Window();

        static Application* Curr();

        Scene* ActiveScene();
        float DeltaTime();
    private:
        static Application* s_Instance;

        GLFWwindow* m_Window = nullptr;

        //PlaneMesh* groundPlane = nullptr;   //TODO: Convert this to entity or get rid of it.
        //SceneGUI* m_Menu = nullptr;
        Scene* m_ActiveScene = nullptr;
        std::unique_ptr<AppGUI> m_AppGUI;

        float m_LastFrameTime = 0.0f; // used to keep track of delta time
        float m_DeltaTime = 0.0f; //Total time for last frame. 
        float m_DesiredFPS = 60.0f;   //FPS will be capped to this value.

        void Inititalize(); // initalize GLFW  
        void LoadScene(Scene* scene); // 
        void FrameTick();

        void Update();
        void Render(); // render UI and scene
        void ImGuiRender();

        void RenderUI();

        void SetupInputCallbacks();
        void ProcessKeyboardInput(GLFWwindow* window, int key, int scancode, int action, int mods);
    };
}