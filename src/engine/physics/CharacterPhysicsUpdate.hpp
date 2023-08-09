#pragma once

#include <btBulletDynamicsCommon.h>
#include "core/Application.hpp"
#include "physics/FindGround.hpp"

namespace lei3d 
{
    class CharacterPhysicsUpdate : public btActionInterface 
    {
    public:
        CharacterPhysicsUpdate(btRigidBody* body);

        void updateAction(btCollisionWorld* collisionWorld, btScalar deltaTime) override;
        void debugDraw(btIDebugDraw* debugDrawer) override;
    private:
        btRigidBody* m_body;

        float m_gravity = 15.24;
        float m_maxAirSpeed = 0.575;
        float m_airAccel = 15000;
        float m_accel = 7.62;
        float m_friction = 4;
        float m_airFriction = 0.25;
        float m_jumpPower = 5.112;
        float m_jumpHeight = 1.5;
        float m_maxSpeed = 6;
        //float m_maxVelocity = 100;

        glm::vec3 Accelerate(glm::vec3 wishDir, glm::vec3 prevVelocity, float acceleration, float maxVelocity);
        glm::vec3 AirAcceleration(glm::vec3 wishDir, glm::vec3 prevVelocity);
        glm::vec3 GroundAcceleration(glm::vec3 wishDir, glm::vec3 prevVelocity);
    };
}