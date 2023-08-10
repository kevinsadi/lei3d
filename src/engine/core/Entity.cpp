#include "Entity.hpp"

#include "logging/Log.hpp"
#include "logging/LogGLM.hpp"

namespace lei3d
{
    Entity::Entity()
    {
        m_Name = "Unnamed";
    }

    Entity::Entity(std::string name)
    {
        m_Name = name;
    }

    Entity::~Entity()
    {
        OnDestroy();
    }

    void Entity::SetPosition(glm::vec3 position)
    {
        m_Transform.position = position; 
    }

    void Entity::SetScale(glm::vec3 scale)
    {
        m_Transform.scale = scale;
    }

    void Entity::SetShader(Shader* shader) {
        m_Shader = shader;
    }

    glm::mat4 Entity::GetTranslationMat() {
        return glm::translate(glm::identity<glm::mat4>(), m_Transform.position);
    }

    glm::mat4 Entity::GetRotationMat() {
        //TODO: Implement quaternion.
        return glm::identity<glm::mat4>();
    }
    
    glm::mat4 Entity::GetScaleMat() {
        return glm::scale(glm::identity<glm::mat4>(), m_Transform.scale);
    }

    const std::string& Entity::GetName()
    {
        return m_Name;
    }

    void Entity::SetName(const std::string& name)
    {
        m_Name = name;
    }

    glm::mat4 Entity::GetModelMat() {
        glm::mat4 translate = GetTranslationMat();
        glm::mat4 rotate = GetRotationMat();
        glm::mat4 scale = GetScaleMat();
        glm::mat4 model = translate * rotate * scale;
        //LEI_INFO("TRANSLATE:");
        //PrintMat4(translate);
        //LEI_INFO("ROTATE:");
        //PrintMat4(rotate);
        //LEI_INFO("SCALE:");
        //PrintMat4(scale);
        //LEI_INFO("MODEL: ");
        //PrintMat4(model);
        return model;
    }

    void Entity::Start() {
        LEI_TRACE("Started Entity");

        for (auto& component : m_Components) {
            component->Start();
        }
    }

    void Entity::Update(float deltaTime) {
        for (auto& component : m_Components) {
            component->Update(deltaTime);
        }
    }

    void Entity::PhysicsUpdate(float deltaTime) {
        for (auto& component : m_Components) {
            component->PhysicsUpdate(deltaTime);
        }
    }

    void Entity::Render() {
        for (auto& component : m_Components) {
            component->Render();
        }
    }

    void Entity::OnDestroy() {
        for (auto& component : m_Components) {
            component->OnDestroy();
        }
    }
}