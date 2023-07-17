#include "engine/Entity.hpp"

namespace lei3d
{
    Entity::Entity() : m_Model(nullptr), m_Shader(nullptr)
    {
        // clown emoticon
    }

    Entity::Entity(Model* model) : m_Model(model) {
        LEI_ASSERT(m_Model);
    }

    Entity::Entity(Model* model, Shader* shader) : m_Model(model), m_Shader(shader)
    {
        LEI_ASSERT(m_Model);
        LEI_ASSERT(m_Shader);
    }

    Entity::~Entity()
    {
        // clown emoticon
    }

    void Entity::AddComponent(EntityComponent component) {

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

    void Entity::SetShader(Shader* shader) {
        m_Shader = shader;
    }

    void Entity::Update(float deltaTime) {
        for (EntityComponent& component : m_Components) {
            component.Update(deltaTime);
        }
    }

    void Entity::Render() {
        m_Model->Draw(*m_Shader);
    }
}