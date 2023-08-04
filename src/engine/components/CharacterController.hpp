#pragma once

#include "engine/Component.hpp"
#include "physics/CharacterPhysicsUpdate.hpp"

namespace lei3d
{
    class Entity;
    class Component;

    class CharacterController : public Component
    {
    public:
        CharacterController(Entity* entity);

        //std::string GetComponentName() override;

        void Start() override;
        void PhysicsUpdate(float deltaTime) override;
        void Init();
    };
}