#pragma once

#include "Entity.hpp"

#include "scenes/Scene.hpp"

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
        Entity* m_Entity;
    public:
        Component(Entity* entity);

        virtual void Start() {}
        virtual void Update(float deltaTime) {}
        virtual void PhysicsUpdate(float deltaTime) {}
        virtual void Render() {}
        virtual void OnDestroy() {}

        Scene& ActiveScene();
    };
}