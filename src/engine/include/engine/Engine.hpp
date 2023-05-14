#define CGLTF_IMPLEMENTATION
#include <cgltf.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include "include/engine/Shader.hpp"
#include "include/engine/FlyCamera.hpp"
#include "include/pcg/PCGHelpers.hpp"

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
        PlaneMesh groundPlane;

        float lastFrame = 0.0f; // used to keep track of delta time
        float deltaTime = 0.0f;

        void Inititalize(); // initalize GLFW  
        void Load(); // load shaders and meshes and other game info?

        void Render(); // render UI and scene
        void RenderScene();
        virtual void RenderUI();

        void processInput(GLFWwindow* window, FlyCamera* camera);
    };
}