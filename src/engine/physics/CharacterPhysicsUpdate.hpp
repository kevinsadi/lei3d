#pragma once

#include "core/Application.hpp"

#include <btBulletDynamicsCommon.h>

namespace lei3d
{
	class CharacterPhysicsUpdate : public btActionInterface
	{
	public:
		CharacterPhysicsUpdate(btRigidBody* character, btCollisionObject* groundCheck, float groundCheckDist);

		void updateAction(btCollisionWorld* collisionWorld, btScalar deltaTime) override;
		void debugDraw(btIDebugDraw* debugDrawer) override;

	private:
		btRigidBody*	   m_Character;
		btCollisionObject* m_GroundCheck;
		float			   m_GroundCheckDist;

		float m_gravity = 15.24f;
		float m_maxAirSpeed = 0.575f;
		float m_airAccel = 800.0f;
		float m_accel = 7.62f;
		float m_friction = 1.0f;
		float m_airFriction = 0.25f;
		float m_jumpPower = 5.112f;
		float m_jumpHeight = 1.5f;
		float m_maxSpeed = 40.0f;
		// float m_maxVelocity = 100;

		bool m_IncludeSFX = true;

		glm::vec3 Accelerate(glm::vec3 wishDir, glm::vec3 prevVelocity, float acceleration, float maxVelocity);
		glm::vec3 AirAcceleration(glm::vec3 wishDir, glm::vec3 prevVelocity);
		glm::vec3 GroundAcceleration(glm::vec3 wishDir, glm::vec3 prevVelocity);
	};
} // namespace lei3d