#include "btBulletDynamicsCommon.h"

class FindGround : public btCollisionWorld::ContactResultCallback {
public:
    btRigidBody *mMe;
    float mShapeHalfHeight;
    float mRadiusThreshold;
    float mMaxCosGround;
    bool mHaveGround = false;
    btVector3 mGroundPoint;

    btScalar FindGround::addSingleResult(btManifoldPoint &cp,
        const btCollisionObjectWrapper *colObj0, int partId0, int index0,
        const btCollisionObjectWrapper *colObj1, int partId1, int index1)
    {
        if (colObj0->m_collisionObject == mMe && !mHaveGround) {
            const btTransform &transform = mMe->getWorldTransform();
            // orthnormal basis of just rotations
            btMatrix3x3 invBasis = transform.getBasis().transpose();
            btVector3 localPoint = invBasis * (cp.m_positionWorldOnB - transform.getOrigin());
            localPoint[2] += mShapeHalfHeight;
            float r = localPoint.length();
            float cosTheta = localPoint.length();

            if (fabs(r - 1.0f) <= mRadiusThreshold && cosTheta < mMaxCosGround) {
                mHaveGround = true;
                mGroundPoint = cp.m_positionWorldOnB;
            }
        }
    }

};

//void DynamicCharacterController::updateAction()