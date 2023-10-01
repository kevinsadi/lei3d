#pragma once
#include <vector>
#include <unordered_set>
#include <btBulletCollisionCommon.h>
#include <set>

namespace lei3d {
	class TriggerColliderCallback : public btCollisionWorld::ContactResultCallback
	{
	public:
		std::unordered_set<const btCollisionObject*>* m_ignoredColliders;
		std::set<const btCollisionObject*>  m_touchingColliders;

		TriggerColliderCallback(std::unordered_set<const btCollisionObject*>* ignoredColliders);
		btScalar addSingleResult(btManifoldPoint& cp,
			const btCollisionObjectWrapper* collObj0, int partId0, int index0,
			const btCollisionObjectWrapper* collObj1, int partId1, int index1) override;
	};

}

