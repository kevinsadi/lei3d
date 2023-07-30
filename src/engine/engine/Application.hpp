#pragma once

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include <iostream>
#include <vector>
#include <filesystem>

#include "pcg/PCGHelpers.hpp"

#include "scenes/Scene.hpp"

namespace lei3d
{
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
    private:
        static Application* s_Instance;

        GLFWwindow* m_Window = nullptr;

        PlaneMesh* groundPlane = nullptr;   //TODO: Convert this to entity assuming we actually use it.

        Scene* m_ActiveScene = nullptr;

        float lastFrameTime = 0.0f; // used to keep track of delta time
        float deltaTime = 0.0f; //Total time for last frame. 
        float desiredFPS = 120.0f;   //FPS will be capped to this value. (current bug means that the FPS cap is half, not sure why)

        void Inititalize(); // initalize GLFW  
        void LoadScene(Scene* scene); // 
        void FrameTick();

        void Update();
        void Render(); // render UI and scene
        void RenderUI(float delaTime); // deltaTime to show FPS

        void SetupSceneCallbacks();
        void processInput(GLFWwindow* window, int key, int scancode, int action, int mods);
    };
}