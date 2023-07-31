#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine/Application.hpp"
#include "engine/Entity.hpp"
#include "engine/FlyCamera.hpp"

#include <memory>
#include <vector>

namespace lei3d
{
    class Application;
    class Entity;
    class PhysicsObjects;
    class Shader;

    class Scene
    {
    protected:
        Application* m_App;
        std::vector<std::unique_ptr<Entity>> m_Entities;

        std::unique_ptr<FlyCamera> m_Camera = nullptr;  //every scene needs a camera

        //glm::mat4 m_VP;
    public:
        Scene();
        ~Scene();

        void Init(Application* app);

        void Start();
        void Update(float deltaTime);
        void PhysicsUpdate(float deltaTime);
        void Render();
        void Destroy();

        void Load();
        void Unload();

        void ProcessCameraInput(float deltaTime);

        //TODO: Abstract scene creation/loading into files: https://trello.com/c/eC66QGuD/25-define-scene-file-format
        //Right now we use this virtual Load function to load all the objs in code. 
        virtual void OnLoad() {}  //load shaders and meshes and other scene info ?
        virtual void OnUnload() {}

        //These should rarely be used because everything is handled by ECS.
        virtual void OnUpdate(float deltaTime) {}
        virtual void OnPhysicsUpdate(float deltaTime) {}
        virtual void OnRender() {}
        virtual void OnDestroy() {}

        FlyCamera& MainCamera();
    private:
        GLFWwindow* window();
    };
}