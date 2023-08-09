#include "physics/FindGround.hpp"

namespace lei3d
{
    FindGround::FindGround(btRigidBody* body) : m_Me(body) {}

    btScalar FindGround::addSingleResult(btManifoldPoint& cp,
                                    const btCollisionObjectWrapper* colObj0, int partId0, int index0,
                                    const btCollisionObjectWrapper* colObj1, int partId1, int index1)
    {
        // We can use this to also check what m_collisionObject2 is to trigger things / change how we react to colliding with it
        if (colObj0->m_collisionObject == m_Me && !m_HaveGround) {
            const btTransform& transform = m_Me->getWorldTransform();
            // Orthonormal basis (just rotations) -> can just transform to invert
            btMatrix3x3 invBasis = transform.getBasis();
            btVector3 localPoint = invBasis * (cp.m_positionWorldOnB - transform.getOrigin());
            localPoint[1] += m_ShapeHalfHeight;
            float r = localPoint.length();
            float cosTheta = localPoint[1] / r;

            if (fabs(r - m_ShapeRadius) <= m_RadiusThreshold && cosTheta < m_MaxCosGround)
            {
                m_HaveGround = true;
                m_GroundPoint = cp.m_positionWorldOnB;
            }
        }
        return 0;
    }
}