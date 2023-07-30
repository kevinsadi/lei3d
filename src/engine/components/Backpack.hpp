#pragma once

#include "engine/Component.hpp"


// THIS IS JUST A TEST COMPONENT. IT HAS NO ACTUAL USE.

namespace lei3d
{
    class Entity;
    class Component;

    class Backpack : public Component
    {
    public:
        Backpack(Entity* entity);

        //std::string GetComponentName() override;

        void Start() override;
    };
}