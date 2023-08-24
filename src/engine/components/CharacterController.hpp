#pragma once

#include "core/Component.hpp"
#include "physics/CharacterPhysicsUpdate.hpp"

namespace lei3d
{
	class Entity;
	class Component;

	class CharacterController : public Component
	{
	private:
		btCollisionShape*	  m_Collider;
		btDefaultMotionState* m_MotionState;
		btRigidBody*		  m_RigidBody;

	public:
		CharacterController(Entity& entity);
		~CharacterController();

		void Start() override;
		void PhysicsUpdate() override;

		void Init();
	};
} // namespace lei3d