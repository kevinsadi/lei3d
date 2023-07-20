#include "Entity.hpp"

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
        OnDestroy();
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

    void Entity::Start() {
        for (Component& component : m_Components) {
            component.Start();
        }
    }

    void Entity::Update(float deltaTime) {
        for (Component& component : m_Components) {
            component.Update(deltaTime);
        }
    }

    void Entity::PhysicsUpdate(float deltaTime) {
        for (Component& component : m_Components) {
            component.PhysicsUpdate(deltaTime);
        }
    }

    void Entity::Render() {
        for (Component& component : m_Components) {
            component.Render();
        }

        //TODO: Abstract into component
        m_Model->Draw(*m_Shader);
    }

    void Entity::OnDestroy() {
        for (Component& component : m_Components) {
            component.OnDestroy();
        }
    }
}