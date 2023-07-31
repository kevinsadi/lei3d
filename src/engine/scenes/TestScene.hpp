#pragma once 

#include "Scene.hpp"


#include "engine/Entity.hpp"

#include "engine/Shader.hpp"
#include "components/SkyBox.hpp"

#include "physics/PhysicsWorld.hpp"

namespace lei3d {
    class Entity;
    class Model;
    class Shader;
    class SkyBox;

    class TestScene : public Scene {
    private:
        PhysicsWorld m_PhysicsWorld;
        std::unique_ptr<Shader> m_MainShader = nullptr; //THIS IS TEMPORARY
    public:
        TestScene();
        ~TestScene();

        void OnLoad() override;
        void OnUpdate(float deltaTime) override;
        void OnPhysicsUpdate(float deltaTime) override;
    };
}