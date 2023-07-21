#pragma once

#include "engine/Component.hpp"

namespace lei3d
{
    class Entity;
    class Component;

    class Backpack : public Component
    {
    public:
        Backpack(Entity* entity);

        void Start() override;
    };
}