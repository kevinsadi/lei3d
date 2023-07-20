#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine/Application.hpp"
#include "engine/Entity.hpp"
#include "engine/FlyCamera.hpp"

#include <vector>

namespace lei3d
{
    class Application;

    class Scene
    {
    protected:
        Application* m_App;
        std::vector<Entity> m_Entities;
        PhysicsObjects m_PhysicsObjects;

        FlyCamera* m_Camera = nullptr;    //every scene needs this
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
        //Right now we use this virtual load function to load all the objs in code. 
        virtual void LoadObjects() {}; //load shaders and meshes and other scene info ?
        virtual void OnUpdate(float deltaTime) {}
        virtual void OnRender() {}
        virtual void OnDestroy() {}

        FlyCamera* MainCamera();
    private:
        GLFWwindow* window();
    };
}