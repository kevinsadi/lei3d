#pragma once

#include "Entity.hpp"

#include <string>

namespace lei3d
{
	class Entity;

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
		virtual void OnReset() {}

#ifdef EDITOR
		virtual void OnEditorUpdate() {}
		virtual void OnImGuiRender() {}
#endif
	};
} // namespace lei3d
