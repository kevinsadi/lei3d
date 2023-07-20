#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine/Component.hpp"
#include "engine/Model.hpp"
#include "engine/Shader.hpp"

#include <unordered_map>

namespace lei3d
{
    struct Transform
    {
        glm::vec3 position;
        glm::vec3 rotation;       
        glm::vec3 scale;
    };

    class Component;
    class Model;
    class Shader;

    class Entity
    {
    private:
        std::unordered_map<std::string, std::vector<Component>> m_ComponentLookup;   //typename, Component
        std::vector<Component> m_Components;
    public:
        Model* m_Model = nullptr;    
        Transform transform;
        Shader* m_Shader = nullptr;



        Entity();
        Entity(Model* model);
        Entity(Model* model, Shader* shader);
        ~Entity();

        virtual void Update(float deltaTime);
        virtual void Render();

        void SetPosition(glm::vec3 translation);
        void SetScale(glm::vec3 scale);

        void SetShader(Shader* shader);

        //Template component stuff

        template<typename C>
        C GetComponent() {
            static_assert(std::is_same<C, Component>::value, "C must be a component type");

            //Get entry in data structure for T           
            //Return first one prob. (may need GetComponents if we have multiple)
        }
        
        template<typename T> 
        void AddComponent(...) {
            //Construct new component object
            //Add new component to dict/whatever data structure for components
        }
    };
}