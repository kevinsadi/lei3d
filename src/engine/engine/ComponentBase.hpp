#pragma once

#include "engine/Component.hpp"
#include "engine/Entity.hpp"

namespace lei3d
{
    class Entity;
    class Component;

    class ComponentBase : Component
    {
    protected:
        Entity* m_Entity = nullptr;
    public:
        ComponentBase(Entity* entity);
        ~ComponentBase();
    };
}