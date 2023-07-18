#ifndef ENGINE_H
#define ENGINE_H

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include <iostream>
#include <vector>
#include <filesystem>

#include "engine/Shader.hpp"
#include "pcg/PCGHelpers.hpp"
#include "engine/FlyCamera.hpp"
#include "engine/Model.hpp"
#include "engine/SkyBox.hpp"
#include "engine/Entity.hpp"
#include "physics/Physics.hpp"

namespace lei3d
{
    class Engine
    {
    public:
        Engine();
        ~Engine();
        
        void Start(); // do everything to start the scene 
    private:
        GLFWwindow* window = nullptr;
        Shader shader;
        FlyCamera* camera = nullptr;
        PlaneMesh* groundPlane = nullptr;
        SkyBox skybox;
        Entity backpackEntity;
        Model* houseModel;
        PhysicsObjects physicsObjects;        

        float lastFrame = 0.0f; // used to keep track of delta time
        float deltaTime = 0.0f;

        void Inititalize(); // initalize GLFW  
        void Load(); // load shaders and meshes and other game info?

        void Render(); // render UI and scene
        void RenderScene();
        void RenderUI(float delaTime); // deltaTime to show FPS

        void processInput(GLFWwindow* window, int key, int scancode, int action, int mods);
        void processCameraInput(float deltaTime);
    };
}

#endif