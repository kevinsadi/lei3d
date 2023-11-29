#include "physics/TriggerCallback.hpp"
#include "physics/PlayerTriggerCallback.hpp"

#include "logging/Log.hpp"
#include <iostream>

namespace lei3d
{
	TriggerCallback::TriggerCallback(btCollisionObject* attachedObj)
		: m_AttachedObj(attachedObj)
	{
	}

	//NOTE: THIS IS ONLY CALLED WHEN A COLLISION OCCURS!
	btScalar TriggerCallback::addSingleResult(btManifoldPoint& cp,
		const btCollisionObjectWrapper* colObj0, int partId0, int index0,
		const btCollisionObjectWrapper* colObj1, int partId1, int index1)
	{
		// We can use this to also check what m_collisionObject2 is to trigger things / change how we react to colliding with it
		

		//We want to check that the colliding object is NOT the player
		const bool objIsPlayer = colObj1->m_collisionObject == m_AttachedObj;
		
		if (!objIsPlayer) {
			void* userPtr = colObj1->m_collisionObject->getUserPointer();
			if (userPtr != nullptr)
				static_cast<PlayerTriggerCallback*>(userPtr)->PlayerEntered();
		}
		
		return 0;
	}
} // namespace lei3d
