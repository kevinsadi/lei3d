#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine/Entity.hpp"
#include "engine/Model.hpp"

namespace lei3d
{
    class Entity;

    class EntityComponent
    {
    protected:
        Entity* m_Entity = nullptr;
    public:

        EntityComponent(Entity* entity);
        ~EntityComponent();

        virtual void Update(float deltaTime) {};
    };
}