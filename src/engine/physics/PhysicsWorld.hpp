// no need for a class, at least right now
// nice to finally get some code going again
#pragma once

#include "btBulletDynamicsCommon.h"

#include <stdio.h>

#include <glm/glm.hpp>
#include <memory>

namespace lei3d
{

	class PhysicsWorld
	{
	public:
		std::unique_ptr<btDefaultCollisionConfiguration>	 m_collisionConfiguration = nullptr;
		std::unique_ptr<btCollisionDispatcher>				 m_dispatcher = nullptr;
		std::unique_ptr<btBroadphaseInterface>				 m_overlappingPairCache = nullptr;
		std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver = nullptr;
		std::unique_ptr<btDiscreteDynamicsWorld>			 m_dynamicsWorld = nullptr;
		btAlignedObjectArray<btCollisionShape*> m_collisionShapes;	//Can we get rid of this??

		PhysicsWorld();

		void Create();
		void Step(float deltaTime);

		// Not marked const bc of bullet weirdness where getters aren't const in the API
		glm::vec3 GetFirstColliderPosition();
	};

} // namespace lei3d