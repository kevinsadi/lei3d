#pragma once 

#include "Scene.hpp"


#include "engine/Entity.hpp"

#include "engine/Shader.hpp"
#include "engine/SkyBox.hpp"

namespace lei3d {
    class Entity;
    class Model;
    class Shader;
    class SkyBox;

    class TestScene : public Scene {
    private:
 
        Entity m_Backpack;
        SkyBox m_Skybox;
    public:
        TestScene();
        ~TestScene();

        void LoadObjects() override;
        void OnUpdate(float deltaTime) override;
        void OnRender() override;
    };
}