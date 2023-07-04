#ifndef ENTITY_H
#define ENTITY_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "include/engine/Model.hpp"

namespace lei3d
{
    struct Transform
    {
        glm::vec3 position;
        glm::vec3 rotation;       
        glm::vec3 scale;
    };

    class Entity
    {
    public:
        Model* model = nullptr;    
        Transform transform;

        Entity();
        Entity(Model* model);
        ~Entity();
    };
}

#endif