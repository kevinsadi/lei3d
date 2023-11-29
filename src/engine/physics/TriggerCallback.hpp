#pragma once

#include <btBulletDynamicsCommon.h>

namespace lei3d
{

	class TriggerCallback : public btCollisionWorld::ContactResultCallback
	{
	public:
		//btRigidBody* m_RigidBody;
		//float		 m_ShapeHalfHeight = 1.5;	   // DEFINED IN CHARACTER CONTROLLER COMPONENT
		//float		 m_ShapeRadius = 1;			   // DEFINED IN CHARACTER CONTROLLER COMPONENT
		//float		 m_RadiusThreshold = 1e-2f;
		//float		 m_MaxCosGround = -SIMDSQRT12; // approx SQRT(1/2) = sqrt(2)/2 , defined by Bullet3
		btCollisionObject* m_AttachedObj;

		//GroundedCallback(btRigidBody* characterBody);
		TriggerCallback(btCollisionObject* attachedObj);

		// ASSUMING A Y UP REFERENCE FRAME I THINK
		btScalar addSingleResult(btManifoldPoint& cp,
			const btCollisionObjectWrapper* colObj0, int partId0, int index0,
			const btCollisionObjectWrapper* colObj1, int partId1, int index1) override;
	};

} // namespace lei3d