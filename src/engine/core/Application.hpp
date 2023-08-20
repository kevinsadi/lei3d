#pragma once

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include <iostream>
#include <filesystem>
#include <memory>
#include <map>
#include <vector>

#include "core/SceneManager.hpp"

#include "guitools/AppGUI.hpp"
#include "rendering/RenderSystem.hpp"

namespace lei3d
{ 
    class AppGUI;
    class SceneManager;

    class Application
    {
    private:
        static Application* s_Instance;

        const unsigned int screenWidth = 1200;
        const unsigned int screenHeight = 1000;
        GLFWwindow* m_Window = nullptr;
        
        std::unique_ptr<AppGUI> m_AppGUI;
        std::unique_ptr<SceneManager> m_SceneManager;

        RenderSystem renderer;

        //NOTE: Don't modify this directly. Use SetUIActive.
        bool m_UIActive = false;

        float m_LastFrameTime = 0.0f; // used to keep track of delta time
        float m_DeltaTime = 0.0f; //Total time for last frame. 
        float m_DesiredFPS = 120.0f;   //FPS will be capped to this value. (current bug means that the FPS cap is half, not sure why)
    public:
        Application();
        ~Application();

        void Run(); // Run the app.

        void SetUIActive(bool uiActive);

        static GLFWwindow* Window();
        static float DeltaTime();
    private:

        void Initialize(); // Start the App
        void FrameTick();   //Called every frame

        void Update();
        void Render();
        void ImGuiRender();

        void SetupInputCallbacks();
        void ProcessKeyboardInput(GLFWwindow* window, int key, int scancode, int action, int mods);
    };
}