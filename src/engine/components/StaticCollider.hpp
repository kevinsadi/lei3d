#pragma once

#include "core/Component.hpp"
#include "rendering/Model.hpp"

namespace lei3d
{
	class Entity;
	class Component;

	class StaticCollider : public Component
	{
	private:
		btScaledBvhTriangleMeshShape* m_Collider;
		btBvhTriangleMeshShape*		  m_NonScaledCollider;
		btMotionState*				  m_MotionState;
		btRigidBody*				  m_RigidBody;

	public:
		StaticCollider(Entity& entity);
		~StaticCollider();

		void Init();
		void SetColliderToModel(Model& model);
		void PhysicsUpdate() override;
		
		btRigidBody* getRigidBody();

	private:
		void AddCollisionsFromTriangleMesh(btTriangleMesh* triMesh, const Transform& transform);
	};
} // namespace lei3d
