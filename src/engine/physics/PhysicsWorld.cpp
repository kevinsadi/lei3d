#include "PhysicsWorld.hpp"

#include "core/Application.hpp"

#include "logging/Log.hpp"

#include "LeiDebugDrawer.hpp"

#include <imgui.h>

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
		m_dynamicsWorld->setGravity(btVector3(0, -m_Gravity, 0));
		m_dynamicsWorld->getSolverInfo().m_numIterations = 30; // Increase the number of solver iterations

		m_debugDrawer = std::make_unique<LeiDebugDrawer>();
		m_debugDrawer->setDebugMode(btIDebugDraw::DBG_DrawAabb);
		m_dynamicsWorld->setDebugDrawer(m_debugDrawer.get());
	}

	void PhysicsWorld::Create()
	{
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

	// We ideally shouldn't use this. Better to store the collider in an object and get the position than assume array order
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

	bool PhysicsWorld::ShouldShowDebug()
	{
		// Don't show physics stuff in game view
		const bool inGameView = Application::GetInstance().GetSceneView().GetMode() == SceneView::ModeGame;
		return m_Debug && !inGameView;
	}

	void PhysicsWorld::OnImGuiRender()
	{
		ImGui::Checkbox("Debug Mode", &m_Debug);
		ImGui::InputFloat("Gravity", &m_Gravity);

		// TODO: This only runs when in GUI. Move this to separate OnDebugRender() or smth. Show only in SceneView and also when scene is paused.
		if (ShouldShowDebug())
		{
			m_dynamicsWorld->debugDrawWorld();
		}
	}
} // namespace lei3d