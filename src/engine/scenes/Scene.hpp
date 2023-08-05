#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/Application.hpp"
#include "core/Entity.hpp"
#include "core/FlyCamera.hpp"
#include "physics/PhysicsWorld.hpp"

#include <memory>
#include <vector>

namespace lei3d
{
    class Application;
    class Entity;
    class PhysicsWorld;
    class Shader;

    class Scene
    {
    protected:
        Application* m_App;
        std::vector<std::unique_ptr<Entity>> m_Entities;

        std::unique_ptr<FlyCamera> m_Camera = nullptr;  // every scene needs a camera
        std::unique_ptr<Shader> m_MainShader = nullptr; // THIS IS TEMPORARY
        std::unique_ptr<PhysicsWorld> m_PhysicsWorld = nullptr; // Each scene has a physics world
    public:
        Scene();
        ~Scene();

        void Init(Application* app);

        void Start();
        void Update(float deltaTime);
        void PhysicsUpdate(float deltaTime);
        void Render();
        void Destroy();

        void ProcessCameraInput(float deltaTime);

        //TODO: Abstract scene creation/loading into files: https://trello.com/c/eC66QGuD/25-define-scene-file-format
        //Right now we use this virtual LoadObjects function to load all the objs in code. 
        virtual void LoadObjects() {}; //load shaders and meshes and other scene info ?

        //These should rarely be used because everything is handled by ECS.
        virtual void OnUpdate(float deltaTime) {}
        virtual void OnPhysicsUpdate(float deltaTime) {}
        virtual void OnRender() {}
        virtual void OnDestroy() {}

        FlyCamera& MainCamera();
        PhysicsWorld& GetPhysicsWorld();
    private:
        GLFWwindow* window();
    };
}