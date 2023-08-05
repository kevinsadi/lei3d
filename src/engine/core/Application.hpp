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

        static Application* Curr();

        void Run(); // Run the app.

        void SetUIActive(bool uiActive);
        void ChangeScenes(Scene* scene);

        GLFWwindow* Window();
        const std::vector<std::pair<std::string, std::unique_ptr<Scene>>>& GetScenes();
        Scene* ActiveScene();
        float DeltaTime();
    private:
        static Application* s_Instance;

        GLFWwindow* m_Window = nullptr;

        //PlaneMesh* groundPlane = nullptr;   //TODO: Convert this to entity or get rid of it.
        //SceneGUI* m_Menu = nullptr;
        Scene* m_ActiveScene = nullptr;
        Scene* m_NextScene = nullptr;
        //std::map<std::string, std::unique_ptr<Scene>> m_AllScenes;
        std::vector<std::pair<std::string, std::unique_ptr<Scene>>> m_AllScenes;
        std::unique_ptr<AppGUI> m_AppGUI;

        //NOTE: Don't modify this directly. Use SetUIActive.
        bool m_UIActive = false;

        bool m_SceneChanged = false;    //Flags the app to load another scene in sync with render loop.

        float m_LastFrameTime = 0.0f; // used to keep track of delta time
        float m_DeltaTime = 0.0f; //Total time for last frame. 
        float m_DesiredFPS = 120.0f;   //FPS will be capped to this value. (current bug means that the FPS cap is half, not sure why)

        void Inititalize(); // Start the App
        void FrameTick();   //Called every frame
        void LoadScene(Scene* scene);

        void Update();
        void Render(); // render UI and scene
        void ImGuiRender();

        void RenderUI();

        void SetupInputCallbacks();
        void ProcessKeyboardInput(GLFWwindow* window, int key, int scancode, int action, int mods);
    };
}