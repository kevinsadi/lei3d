#include "PhysicsWorld.hpp"

#include "LeiDebugDrawer.hpp"

namespace lei3d
{
	PhysicsWorld::PhysicsWorld()
	{
		// this is just the default setup, we can make this fancier if we want
		m_collisionConfiguration = std::make_unique<btDefaultCollisionConfiguration>();
		m_dispatcher = std::make_unique<btCollisionDispatcher>(m_collisionConfiguration.get());

		// good general purpose broadphase, we cna also use btAxis3Sweep
		m_overlappingPairCache = std::make_unique<btDbvtBroadphase>();
		m_solver = std::make_unique<btSequentialImpulseConstraintSolver>();

		m_dynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(m_dispatcher.get(), m_overlappingPairCache.get(), m_solver.get(), m_collisionConfiguration.get());
		m_dynamicsWorld->setGravity(btVector3(0, -10, 0));

		m_debugDrawer = std::make_unique<LeiDebugDrawer>();
		m_dynamicsWorld->setDebugDrawer(m_debugDrawer.get());
	}

	void PhysicsWorld::Create()
	{
		// CHARACTER--------------------
		// std::unique_ptr<btCollisionShape> character = std::make_unique<btCapsuleShape>(btScalar{1.0f}, btScalar{3.0f});
		//  btCollisionShape* character = new btCapsuleShape(btScalar{1.0f}, btScalar{3.0f});
		//  btTransform startTransform;
		//  startTransform.setIdentity();

		// btScalar mass{1.f};
		// btVector3 localInertia{0.0f, 0.0f, 0.0f};
		// character->calculateLocalInertia(mass, localInertia);
		// startTransform.setOrigin(btVector3{3.0f, 50.0f, 0.0f});

		// //THIS IS A MEMORY LEAK, FIX!!
		// btDefaultMotionState* charMotionState = new btDefaultMotionState(startTransform);
		// btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, charMotionState, character, localInertia);
		// btRigidBody* characterBody = new btRigidBody(rbInfo);
		// m_dynamicsWorld->addRigidBody(characterBody);
		// m_collisionShapes.push_back(character);
	}

	void PhysicsWorld::Step(float deltaTime)
	{
		m_dynamicsWorld->stepSimulation(deltaTime, 10);

		// Move every object
		for (int j = m_dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
		{
			btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[j];
			btRigidBody*	   body = btRigidBody::upcast(obj);
			btTransform		   trans;
			if (body && body->getMotionState())
			{
				body->getMotionState()->getWorldTransform(trans);
			}
			else
			{
				trans = obj->getWorldTransform();
			}
		}
	}

	//We ideally shouldn't use this. Better to store the collider in an object and get the position than assume array order
	glm::vec3 PhysicsWorld::GetFirstColliderPosition()
	{
		btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[0];
		btRigidBody*	   body = btRigidBody::upcast(obj);
		btTransform		   trans;
		if (body && body->getMotionState())
		{
			body->getMotionState()->getWorldTransform(trans);
		}
		else
		{
			trans = obj->getWorldTransform();
		}
		glm::vec3 position = glm::vec3(float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
		return position;
	}

	void PhysicsWorld::OnImGuiRender()
	{
	}
} // namespace lei3d