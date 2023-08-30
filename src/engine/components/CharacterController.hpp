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
		float				  m_Width, m_Height;
		btCollisionShape*	  m_Collider;
		btDefaultMotionState* m_MotionState;
		btRigidBody*		  m_RigidBody;

		// GROUND CHECK
		btVector3		   m_GroundCheckLocalPos;
		btScalar		   m_GroundCheckDist;
		btCollisionShape*  m_GroundCheckCollider;
		btCollisionObject* m_GroundCheckObj;

	public:
		CharacterController(Entity& entity);
		~CharacterController();

		void Init(float width, float height, float groundCheckDist = 1.0f, glm::vec3 groundCheckLocalPos = { 0.0f, -0.5f, 0.0f });

		void Start() override;
		void PhysicsUpdate() override;

	private:
		btTransform getGroundCheckTransform(const btTransform& parentTransform);
	};
} // namespace lei3d