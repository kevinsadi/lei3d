#pragma once 

#include "core/Scene.hpp"


#include "core/Entity.hpp"

#include "rendering/Shader.hpp"
#include "components/SkyBox.hpp"
#include "components/StaticCollider.hpp"
#include "components/CharacterController.hpp"

#include "physics/PhysicsWorld.hpp"

#include "audio/AudioPlayer.hpp"

namespace lei3d {
    class Entity;
    class Model;
    class Shader;
    class SkyBox;
    class PhysicsWorld;

    class TestSceneKevin : public Scene {
    public:
        TestSceneKevin();
        ~TestSceneKevin();

        void OnLoad() override;
        void OnUpdate() override;
        void OnPhysicsUpdate() override;
        void OnRender() override;
    private:
        std::unique_ptr<Model> backpackModel;
        std::unique_ptr<Model> playgroundModel;
        std::unique_ptr<AudioPlayer> m_AudioPlayer;
    };
}