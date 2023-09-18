#pragma once

#include "core/Component.hpp"

#include <btBulletDynamicsCommon.h>

namespace lei3d
{
	class Entity;
	class Component;

	class CharacterController : public Component
	{
	public:
		// CHARACTER CONTROLLER PARAMETERS
		float m_accel = 50.0f;
		float m_airAccel = 1000.0f;
		float m_maxSpeed = 500.0f;
		float m_maxAirSpeed = 20.575f;

		float m_friction = 1.0f;
		//float m_airFriction = 0.25f;

		float m_jumpPower = 3.f;
		//float m_jumpHeight = 1.f;

		// float m_maxSpeed = 40.0f;
		//  float m_maxVelocity = 100;

		bool m_IsInDynamicsWorld;

	private:
		class CharacterPhysicsUpdate : public btActionInterface
		{
		private:
			CharacterController& m_Controller;
			btRigidBody*		 m_Character;
			btCollisionObject*	 m_GroundCheck;
			float				 m_GroundCheckDist;

		public:
			CharacterPhysicsUpdate(CharacterController& controller, btRigidBody* character, btCollisionObject* groundCheck, float groundCheckDist);

			void updateAction(btCollisionWorld* collisionWorld, btScalar deltaTime) override;
			void debugDraw(btIDebugDraw* debugDrawer) override;

		private:
			glm::vec3 Accelerate(glm::vec3 wishDir, glm::vec3 prevVelocity, float acceleration, float maxVelocity);
			glm::vec3 AirAcceleration(glm::vec3 wishDir, glm::vec3 prevVelocity);
			glm::vec3 GroundAcceleration(glm::vec3 wishDir, glm::vec3 prevVelocity);
		};

		float					m_Width, m_Height;
		btCollisionShape*		m_Collider;
		btDefaultMotionState*	m_MotionState;
		btRigidBody*			m_RigidBody;
		CharacterPhysicsUpdate* m_CharacterPhysicsUpdate;

		// GROUND CHECK
		btVector3		   m_GroundCheckLocalPos;
		btScalar		   m_GroundCheckDist;
		btCollisionShape*  m_GroundCheckCollider;
		btCollisionObject* m_GroundCheckObj;

		bool m_Grounded;
		bool m_IncludeSFX = true;

	public:
		CharacterController(Entity& entity);
		~CharacterController();

		void Init(float width, float height, float groundCheckDist = 1.f);

		void Start() override;
		void Update() override;
		void PhysicsUpdate() override;
		void OnImGuiRender() override;
		void OnReset() override;

		bool IsGrounded() const;

	private:
		btTransform getGroundCheckTransform(const btTransform& parentTransform);
	};
} // namespace lei3d