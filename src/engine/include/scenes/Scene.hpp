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
        std::vector<std::shared_ptr<Entity>> m_Entities;
        PhysicsObjects m_PhysicsObjects;

        FlyCamera* m_Camera = nullptr;    //every scene needs this
    public:
        Scene();
        ~Scene();

        void Init(Application* app);

        void Update(float deltaTime);
        void Render();
        void ProcessCameraInput(float deltaTime);

        //TODO: Abstract scene creation/loading into files: https://trello.com/c/eC66QGuD/25-define-scene-file-format
//Right now we use this virtual load function to load all the objs in code. 
        virtual void LoadObjects() {}; //load shaders and meshes and other scene info ?
        virtual void OnUpdate() {}
        virtual void OnRender() {}

        FlyCamera* MainCamera();
    private:
        void initCamera();
        GLFWwindow* window();
    };
}