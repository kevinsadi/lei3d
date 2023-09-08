#include "physics/GroundedCallback.hpp"

#include "logging/Log.hpp"

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

		////We only want the flag to be set when colliding with non-player geometry.
		//// we also want to check the angle of collision

		btVector3 collisionAngle = cp.m_normalWorldOnB;
		//std::cout << collisionAngle.getX() << " " << collisionAngle.getY() << " " << collisionAngle.getZ() << std::endl;

		//bool angleIsGood = collisionAngle < -50;

		m_Grounded |= !objIsPlayer && collisionAngle.getY() < -0.5; // && angleIsGood; //  syntactic sugar for a = a | b;

		//if (colObj0->m_collisionObject == m_RigidBody && !m_Grounded)
		//{
		//	const btTransform& transform = m_RigidBody->getWorldTransform();
		//	// Orthonormal basis (just rotations) -> can just transform to invert
		//	btMatrix3x3 invBasis = transform.getBasis();
		//	btVector3	localPoint = invBasis * (cp.m_positionWorldOnB - transform.getOrigin());
		//	localPoint[1] += m_ShapeHalfHeight;
		//	float r = localPoint.length();
		//	float cosTheta = localPoint[1] / r;

		//	if (fabs(r - m_ShapeRadius) <= m_RadiusThreshold && cosTheta < m_MaxCosGround)
		//	{
		//		m_Grounded = true;
		//		m_GroundPoint = cp.m_positionWorldOnB;
		//	}
		//}
		return 0;
	}
} // namespace lei3d