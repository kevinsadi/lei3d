#pragma once

#include "engine/Component.hpp"
#include "components/Model.hpp"

namespace lei3d
{
    class Entity;
    class Component;

    class StaticCollider : public Component
    {
    public:
        StaticCollider(Entity* entity);

        void Init();
    private:
        void GiveModelStaticCollider();
        void AddCollisionsFromTriangleMesh(btTriangleMesh* triMesh, Transform transform);
    };
}