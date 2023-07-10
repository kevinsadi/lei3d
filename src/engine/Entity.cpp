#include "include/engine/Entity.hpp"

namespace lei3d
{
    Entity::Entity()
    {
        // clown emoticon
    }

    Entity::Entity(Model* model) : model(model)
    {
        // We have moved on to member intializer lists
    }

    Entity::~Entity()
    {
        // clown emoticon
    }

    // Translate entity by glm::vec3
    void Entity::SetPosition(glm::vec3 position)
    {
        transform.position = position; 
    }

    // Multiply the 
    void Entity::SetScale(glm::vec3 scale)
    {
        transform.scale = scale;
    }
}