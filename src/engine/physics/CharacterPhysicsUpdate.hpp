#pragma once

#include <btBulletDynamicsCommon.h>
#include "core/Application.hpp"

namespace lei3d {
    class CharacterPhysicsUpdate : public btActionInterface {
    public:
        CharacterPhysicsUpdate(btRigidBody* body);

        void updateAction(btCollisionWorld* collisionWorld, btScalar deltaTime) override;
        void debugDraw(btIDebugDraw* debugDrawer) override;
    private:
        btRigidBody* m_body;

        float m_airCap = 0.575;
        float m_airAccel = 1000;
        float m_accel = 7.62f;



    };
}