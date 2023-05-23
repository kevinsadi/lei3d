#ifndef ENGINE_H
#define ENGINE_H

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <filesystem>

#include "include/engine/Shader.hpp"
#include "include/pcg/PCGHelpers.hpp"
#include "include/engine/FlyCamera.hpp"
#include "include/engine/Model.hpp"

namespace kek3d
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
        Model* meshModel = nullptr;

        float lastFrame = 0.0f; // used to keep track of delta time
        float deltaTime = 0.0f;

        void Inititalize(); // initalize GLFW  
        void Load(); // load shaders and meshes and other game info?

        void Render(); // render UI and scene
        void RenderScene();
        //virtual void RenderUI();

        void processInput(GLFWwindow* window, FlyCamera* camera);
    };
}

#endif