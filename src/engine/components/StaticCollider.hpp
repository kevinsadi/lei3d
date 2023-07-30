#pragma once

#include "engine/Component.hpp"

namespace lei3d
{
    class Entity;
    class Component;

    class StaticCollider : public Component
    {
    public:
        StaticCollider(Entity* entity);

        void Start() override;
    };
}