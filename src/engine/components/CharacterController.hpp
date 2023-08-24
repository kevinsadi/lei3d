#pragma once

#include "core/Component.hpp"
#include "physics/CharacterPhysicsUpdate.hpp"

namespace lei3d
{
	class Entity;
	class Component;

	class CharacterController : public Component
	{
	public:
		CharacterController(Entity& entity);

		void Start() override;
		void PhysicsUpdate() override;

		void Init();
	};
} // namespace lei3d