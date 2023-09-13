#include "StaticCollider.hpp"

#include "core/SceneManager.hpp"
#include "logging/Log.hpp"

namespace lei3d
{
	StaticCollider::StaticCollider(Entity& entity)
		: Component(entity)
	{
	}

	StaticCollider::~StaticCollider()
	{
		delete m_Collider;
		delete m_NonScaledCollider;
		delete m_MotionState;
		delete m_RigidBody;
	}

	/**
	 * @brief - Instantiate Static Collider on an entity. If it has a model.
	 *
	 * Adds collider mesh to the physics object of the current scene.
	 *
	 */
	void StaticCollider::Init()
	{
	}

	void StaticCollider::SetColliderToModel(Model& model)
	{
		std::vector<btTriangleMesh*>& modelMeshes = model.GetCollisionMeshes();
		for (auto triMesh : modelMeshes)
		{
			AddCollisionsFromTriangleMesh(triMesh, m_Entity.m_Transform);
		}
	}

	/**
	 * @brief Mutates values in PhysicsWorld to add the triMesh to the dynamicsWorldScene
	 *
	 * NOTE: triMesh not passed by const bc btBvhTriangleMeshShape does n ot  take in const
	 *
	 * @param triMesh
	 * @param transform
	 */
	void StaticCollider::AddCollisionsFromTriangleMesh(btTriangleMesh* triMesh, const Transform& transform)
	{
		btVector3 scaleVector{ transform.scale.x, transform.scale.y, transform.scale.z };
		m_NonScaledCollider = new btBvhTriangleMeshShape(triMesh, true, true);
		m_Collider = new btScaledBvhTriangleMeshShape(m_NonScaledCollider, scaleVector);

		// now add this mesh to our physics world.
		PhysicsWorld& world = SceneManager::ActiveScene().GetPhysicsWorld();
		//.m_collisionShapes.push_back(m_Collider);

		btTransform meshTransform;
		meshTransform.setIdentity();
		meshTransform.setOrigin(btVector3{ transform.position.x, transform.position.y, transform.position.z });

		// mesh environment collisions are static
		btScalar  meshMass = 0.0f;
		btVector3 meshLocalInertia{ 0.0f, 0.0f, 0.0f };

		m_MotionState = new btDefaultMotionState(meshTransform);
		btRigidBody::btRigidBodyConstructionInfo rbMeshInfo{ meshMass, m_MotionState, m_Collider, meshLocalInertia };
		m_RigidBody = new btRigidBody(rbMeshInfo);

		world.m_dynamicsWorld->addRigidBody(m_RigidBody);
	}

	// FOR SOME REASON THE COLLISION MESH DOESN'T CHANGE
	void StaticCollider::PhysicsUpdate()
	{
		btTransform trans;
		// if entity pos changed
		if (m_Entity.m_ResetTransform) {
			m_Entity.m_ResetTransform = false;
			trans = m_Entity.getBTTransform();
			m_RigidBody->setWorldTransform(trans);
			m_MotionState->setWorldTransform(trans);
			m_Entity.setFromBTTransform(trans);
		}
	}
} // namespace lei3d
