#pragma once

#include "Entity.hpp"
#include "core/Scene.hpp"

#include <string>

namespace lei3d
{
	class Entity;
	class Scene;

	class Component
	{
	protected:
		Entity& m_Entity;

	public:
		Component(Entity& entity);

		virtual void Start() {}
		virtual void Update() {}
		virtual void PhysicsUpdate() {}
		virtual void Render() {}
		virtual void OnDestroy() {}

		virtual void OnEditorUpdate() {}
	};
} // namespace lei3d
