#include "physics/GroundedCallback.hpp"
#include "physics/PlayerTriggerCallback.hpp"

#include "logging/Log.hpp"
#include <iostream>

namespace lei3d
{
	//GroundedCallback::GroundedCallback(btRigidBody* body)
	//	: m_RigidBody(body) {}

	GroundedCallback::GroundedCallback(btCollisionObject* attachedObj)
		: m_AttachedObj(attachedObj), m_Grounded(false)
	{
	}

	//NOTE: THIS IS ONLY CALLED WHEN A COLLISION OCCURS!
	btScalar GroundedCallback::addSingleResult(btManifoldPoint& cp,
		const btCollisionObjectWrapper* colObj0, int partId0, int index0,
		const btCollisionObjectWrapper* colObj1, int partId1, int index1)
	{
		// We can use this to also check what m_collisionObject2 is to trigger things / change how we react to colliding with it
		

		//We want to check that the colliding object is NOT the player
		const bool objIsPlayer = colObj1->m_collisionObject == m_AttachedObj;

		btVector3 collisionAngle = cp.m_normalWorldOnB;

		// for testing
		// if (!objIsPlayer)
		// {
		// 	std::cout << collisionAngle.getX() << " " << collisionAngle.getY() << " " << collisionAngle.getZ() << std::endl;
		// 	collisionAngle.angle(btVector3(0, 1, 0));
		// }

		////We only want the flag to be set when colliding with non-player geometry.
		//// we also want to check the angle of collision
		m_Grounded |= !objIsPlayer && collisionAngle.getY() < -0.95; // && angleIsGood; //  syntactic sugar for a = a | b;
		return 0;
	}
} // namespace lei3d
