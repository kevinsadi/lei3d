#ifndef ENTITY_H
#define ENTITY_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine/Model.hpp"
#include "physics/Physics.hpp"
#include "util/Transform.hpp"

namespace lei3d
{
    class Entity
    {
    public:
        Model* model = nullptr;    
        Transform transform;

        Entity();
        Entity(Model* model);
        ~Entity();

        void SetPosition(glm::vec3 translation);
        void SetScale(glm::vec3 scale);
        void AddModelColliderStatic(PhysicsObjects physicsObjects);
    };
}

#endif