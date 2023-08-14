#pragma once

#include "core/Component.hpp"
#include "logging/Log.hpp"
#include "rendering/Shader.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <string>
#include <typeinfo>
#include <unordered_map>

namespace lei3d
{
	struct Transform
	{
		glm::vec3 position;
		// glm::vec3 rotation;
		glm::vec3 scale;
	};

	class Component;
	class Shader;

	class Entity
	{
	private:
		std::vector<std::unique_ptr<Component>> m_Components;
		std::string								m_Name;

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

        void OnEditorUpdate();

        glm::mat4 GetTranslationMat() const;
        glm::mat4 GetRotationMat() const;
        glm::mat4 GetScaleMat() const;
        glm::mat4 GetModelMat() const;

        void SetPosition(const glm::vec3& position);
        void SetScale(const glm::vec3& scale);

        //TODO: Consider refactoring Editor GUIs to separate class
        void NameGUI();
        void TransformGUI();
        void ShowInspectorGUI();

		const std::string& GetName() const;
		void			   SetName(const std::string& name);

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
		template <typename C>
		C* GetComponent()
		{
			static_assert(std::is_convertible<C, Component>::value, "C must be a component type");

            //(may need GetComponents if we have multiple)
            for (auto& c : m_Components) {
                //Component& cRef = *c;
                if (auto* casted = dynamic_cast<C*>(c.get())) {
                    //returns the first match
                    return static_cast<C*>(c.get());
                }
            }

			// LEI_ERROR("Could not find component.");
			return nullptr;
		}

		/*
		 * Creates and adds a component of type C and returns a reference to it.
		 * Internally calls the component constructor.
		 */
		template <typename C>
		C* AddComponent()
		{
			// If this is hitting, check that your component is using "public" for inheritance.
			static_assert(std::is_convertible<C, Component>::value, "C must be a component type.");

			std::unique_ptr<C> c = std::make_unique<C>(*this);
			m_Components.push_back(std::move(c));
			// The element was just added at the back of the component list, so we retrieve it from the back here.
			// Note: We cannot use c since it got moved into the list.
			return static_cast<C*>(m_Components.back().get());
		}
	};
} // namespace lei3d
