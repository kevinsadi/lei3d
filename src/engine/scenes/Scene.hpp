#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/Entity.hpp"
#include "core/FlyCamera.hpp"

#include "physics/PhysicsWorld.hpp"

#include <memory>
#include <vector>
#include <unordered_map>

namespace lei3d
{
    class Application;
    class Entity;
    class FlyCamera;
    class PhysicsWorld;
    class Shader;
    class RenderSystem;

    class Scene
    {
    private:
        friend RenderSystem;

        std::vector<std::unique_ptr<Entity>> m_Entities;
        std::unordered_map<std::string, int> m_EntityNameCounts;
    protected:
        Application* m_App;

        //We should prob. limit how much stuff we put into the base scene.
        std::unique_ptr<FlyCamera> m_Camera = nullptr;  // every scene needs a camera
        std::unique_ptr<Shader> m_MainShader = nullptr; // THIS IS TEMPORARY
        std::unique_ptr<PhysicsWorld> m_PhysicsWorld = nullptr; // Each scene has a physics world
    public:
        Scene();
        ~Scene();

        void Init(Application* app);
        
        //Entities
        Entity& AddEntity(std::string name);
        Entity& AddEntity();
        Entity* GetEntity(std::string name);

        //Entity Messages
        void Start();
        void Update(float deltaTime);
        void PhysicsUpdate(float deltaTime);
        void Render();
        void ImGUIRender();
        void Destroy();

        void Load();
        void Unload();

        //TODO: Abstract scene creation/loading into files: https://trello.com/c/eC66QGuD/25-define-scene-file-format
        //Right now we use this virtual Load function to load all the objs in code. 
        virtual void OnLoad() {}  //load shaders and meshes and other scene info ?
        virtual void OnUnload() {}

        //These should rarely be used because everything is handled by ECS.
        virtual void OnUpdate(float deltaTime) {}
        virtual void OnPhysicsUpdate(float deltaTime) {}
        virtual void OnRender() {}
        virtual void OnImGUIRender() {}
        virtual void OnDestroy() {}

        FlyCamera& MainCamera() const;
        PhysicsWorld& GetPhysicsWorld();

        void PrintEntityList();
    private:
        GLFWwindow* window();
    };
}