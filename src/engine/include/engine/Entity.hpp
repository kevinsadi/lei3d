#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine/Model.hpp"
#include "engine/EntityComponent.hpp"

namespace lei3d
{
    struct Transform
    {
        glm::vec3 position;
        glm::vec3 rotation;       
        glm::vec3 scale;
    };

    class EntityComponent;

    class Entity
    {
    public:
        Model* m_Model = nullptr;    
        Transform transform;
        Shader* m_Shader = nullptr;

        std::vector<EntityComponent> m_Components;

        Entity();
        Entity(Model* model);
        Entity(Model* model, Shader* shader);
        ~Entity();

        void AddComponent(EntityComponent component);

        virtual void Update(float deltaTime);
        virtual void Render();

        void SetPosition(glm::vec3 translation);
        void SetScale(glm::vec3 scale);

        void SetShader(Shader* shader);
    };
}