#include "Entity.hpp"

#include "util/Log.hpp"

namespace lei3d
{
    Entity::Entity()
    {
        // clown emoticon
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
        LEI_TRACE("Started Entity");

        for (auto component : m_Components) {
            component->Start();
        }
    }

    void Entity::Update(float deltaTime) {
        for (auto component : m_Components) {
            component->Update(deltaTime);
        }
    }

    void Entity::PhysicsUpdate(float deltaTime) {
        for (auto component : m_Components) {
            component->PhysicsUpdate(deltaTime);
        }
    }

    void Entity::Render() {
        for (auto component : m_Components) {
            component->Render();
        }

        //TODO: Abstract into component
        //m_Model->Draw(*m_Shader);
    }

    void Entity::OnDestroy() {
        for (auto component : m_Components) {
            component->OnDestroy();
        }
    }
}