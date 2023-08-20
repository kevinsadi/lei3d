#pragma once

#include "Entity.hpp"

#include "core/Scene.hpp"

#include <string>

namespace lei3d
{


//#define DEFINE_COMPONENT(type, name) \
//template<> \
//std::string GetComponentName<type>() { return std::string(name); }

    class Entity;
    class Scene;

    class Component
    {
    protected:
        Entity& m_Entity;
    public:
        Component(Entity& entity);

        virtual void Start() {}
        virtual void Update() {}
        virtual void PhysicsUpdate() {}
        virtual void Render() {}
        virtual void OnDestroy() {}
    };
}