#include "TriggerColliderCallback.hpp"

namespace lei3d
{
	TriggerColliderCallback::TriggerColliderCallback(std::unordered_set<const btCollisionObject*>* ignoredColliders)
		: m_ignoredColliders(ignoredColliders)
	{
	}

	btScalar TriggerColliderCallback::addSingleResult(btManifoldPoint& cp, 
		const btCollisionObjectWrapper* collObj0, int partId0, int index0, 
		const btCollisionObjectWrapper* collObj1, int partId1, int index1)
	{
		auto* colObj = collObj1->m_collisionObject;
		if (!(m_ignoredColliders->contains(colObj))) {
			m_touchingColliders.push_back(colObj);
		}

		return 0;
	}
} // namespace lei3d