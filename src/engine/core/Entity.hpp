#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/Component.hpp"

#include "logging/Log.hpp"

#include "rendering/Shader.hpp"

#include <memory>
#include <typeinfo>
#include <unordered_map>
#include <string>

namespace lei3d
{
    struct Transform
    {
        glm::vec3 position;
        //glm::vec3 rotation;       
        glm::vec3 scale;
    };

    class Component;
    class Shader;

    class Entity
    {
    private:
        std::vector<std::unique_ptr<Component>> m_Components;
        std::string m_Name;
    public:
        Transform m_Transform;

        Entity();
        Entity(const std::string& name);

        ~Entity();

        void Start();
        void Update();
        void PhysicsUpdate();
        void Render();
        void OnDestroy();

        void SetPosition(const glm::vec3& position);
        void SetScale(const glm::vec3& scale);

        glm::mat4 GetTranslationMat();
        glm::mat4 GetRotationMat();
        glm::mat4 GetScaleMat();
        glm::mat4 GetModelMat();

        const std::string& GetName();
        void SetName(const std::string& name);

        /*
        * Component System:
        * Components should always be added through AddComponent<C> and returned through GetComponent<C>.
        * These work essentially the same as in Unity. 
        * DO NOT Call the Constructor for a Component or any of it's subclasses. Always use add Component.
        * If you need to initialize a component with data, use an Init function (see SkyBox.cpp for an example)
        */

        /*
        * Will return component attached to this entity of type C.
        * If there are multiple components of the same type, it will return the first one found. 
        */
        template<typename C>
        C* GetComponent() {
            static_assert(std::is_convertible<C, Component>::value, "C must be a component type");

            //(may need GetComponents if we have multiple)
            for (auto& c : m_Components) {
                //Component& cRef = *c;
                if (typeid(*c)  == typeid(C)) {
                    //returns the first match
                    return static_cast<C*>(c.get());
                }
            }

            LEI_ERROR("Could not find component.");
            return nullptr;
        }
        
        /*
        * Creates and adds a component of type C and returns a reference to it.
        * Internally calls the component constructor.
        */
        template<typename C> 
        C* AddComponent() {

            //If this is hitting, check that your component is using "public" for inheritance.
            static_assert(std::is_convertible<C, Component>::value, "C must be a component type.");

            std::unique_ptr<C> c = std::make_unique<C>(*this);
            m_Components.push_back(std::move(c));
            //The element was just added at the back of the component list, so we retrieve it from the back here. 
            //Note: We cannot use c since it got moved into the list.
            return static_cast<C*>(m_Components.back().get());
        }

        void ShowInspectorGUI();
    };
}