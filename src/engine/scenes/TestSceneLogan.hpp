#pragma once 

#include "core/Entity.hpp"
#include "core/Scene.hpp"

#include "rendering/Shader.hpp"
#include "components/SkyBox.hpp"
#include "components/StaticCollider.hpp"
#include "components/CharacterController.hpp"

#include "physics/PhysicsWorld.hpp"

namespace lei3d {
    class Entity;
    class Model;
    class Shader;
    class SkyBox;
    class PhysicsWorld;

    class TestSceneLogan : public Scene {
    public:
        TestSceneLogan();
        ~TestSceneLogan();

        void OnLoad() override;
        void OnPhysicsUpdate() override;
    private:
        std::unique_ptr<Model> backpackModel;
    };
}