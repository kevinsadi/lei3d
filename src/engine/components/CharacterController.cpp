#include "CharacterController.hpp"

namespace lei3d
{
	// DEFINE_COMPONENT(Backpack, "Backpack");

	CharacterController::CharacterController(Entity& entity)
		: Component(entity)
	{
	}

	CharacterController::~CharacterController()
	{
		delete m_Collider;
		delete m_MotionState;
		delete m_RigidBody;
	}

	void CharacterController::Start()
	{
		LEI_TRACE("Character Controller started");
	}

	/**
	 * Add a cylinder collision mesh to this object. DO THIS BEFORE MAKING ANY OTHER MESHES.
	 * Also cannot change the size of the graphics mesh of the character controller for proper collisions.
	 *
	 */
	void CharacterController::Init()
	{
		// CHARACTER--------------------
		m_Collider = new btCapsuleShape(btScalar{ 1.0f }, btScalar{ 3.0f });
		btTransform startTransform;
		startTransform.setIdentity();

		btScalar  mass{ 1.f };
		btVector3 localInertia{ 0.0f, 0.0f, 0.0f };
		m_Collider->calculateLocalInertia(mass, localInertia);
		Transform transform = m_Entity.m_Transform;
		startTransform.setOrigin(btVector3{ transform.position.x, transform.position.y, transform.position.z });

		m_MotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, m_MotionState, m_Collider, localInertia);
		m_RigidBody = new btRigidBody(rbInfo);
		m_RigidBody->setSleepingThresholds(0.0, 0.0);
		m_RigidBody->setAngularFactor(0.0);

		PhysicsWorld& world = SceneManager::ActiveScene().GetPhysicsWorld();
		world.m_dynamicsWorld->addRigidBody(m_RigidBody);
		world.m_collisionShapes.push_back(m_Collider);

		// WITHIN THIS CUSTOM PHYSICS UPDATE IS THE MAGIC THAT MAKES AIRSTRAFING / SURF POSSIBLE
		CharacterPhysicsUpdate* customCharacterPhysicsUpdate = new CharacterPhysicsUpdate(m_RigidBody);
		world.m_dynamicsWorld->addAction(customCharacterPhysicsUpdate);
	}

	void CharacterController::PhysicsUpdate()
	{
		m_Entity.m_Transform.position = SceneManager::ActiveScene().GetPhysicsWorld().GetFirstColliderPosition();
	}

} // namespace lei3d