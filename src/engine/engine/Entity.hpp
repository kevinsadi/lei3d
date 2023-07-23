#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "engine/Component.hpp"
#include "engine/Shader.hpp"

#include <memory>
#include <typeinfo>
#include <unordered_map>
#include <string>

namespace lei3d
{
    struct Transform
    {
        glm::vec3 position;
        glm::vec3 rotation;       
        glm::vec3 scale;
    };

    class Component;
    class Shader;

    class Entity
    {
    private:
        std::vector<std::shared_ptr<Component>> m_Components;
    public:
        Transform transform;
        Shader* m_Shader = nullptr;

        Entity();
        ~Entity();

        void Start();
        void Update(float deltaTime);
        void PhysicsUpdate(float deltaTime);
        void Render();
        void OnDestroy();

        void SetPosition(glm::vec3 translation);
        void SetScale(glm::vec3 scale);

        void SetShader(Shader* shader);

        //Template component stuff

        template<typename C>
        C* GetComponent() {
            static_assert(std::is_convertible<C, Component>::value, "C must be a component type");

            //Get entry in data structure for T           
            //Return first one prob. (may need GetComponents if we have multiple)
            for (Component& c : m_Components) {
                if (GetComponentName<typeid(c)>.compare(GetComponentName<C>()) == 0) {
                    //returns the first match
                    return &c;
                }
            }
        }
        
        //returns the component that was just created.
        template<typename C> 
        std::shared_ptr<C> AddComponent() {

            //std::string componentTypeStr = "Must be a component type: ";
            //componentTypeStr.append(GetComponentName<C>());

            static_assert(std::is_convertible<C, Component>::value, "C must be a component type.");

            std::shared_ptr<C> c = std::make_shared<C>(this);
            m_Components.push_back(c);
            return c;
        }
    };
}