// no need for a class, at least right now
// nice to finally get some code going again
#pragma once

#include "btBulletDynamicsCommon.h"
#include <stdio.h>
#include <glm/glm.hpp>

#include <memory>

namespace lei3d
{
    struct PhysicsWorld
    {
        std::unique_ptr<btDefaultCollisionConfiguration> collisionConfiguration;
        std::unique_ptr<btCollisionDispatcher> dispatcher = nullptr;
        std::unique_ptr<btBroadphaseInterface> overlappingPairCache = nullptr;
        std::unique_ptr<btSequentialImpulseConstraintSolver> solver = nullptr;
        std::unique_ptr<btDiscreteDynamicsWorld> dynamicsWorld = nullptr;
        //btAlignedObjectArray<std::unique_ptr<btCollisionShape>> collisionShapes;
        btAlignedObjectArray<btCollisionShape*> collisionShapes;

        PhysicsWorld();
        void Create();

        void Step(float deltaTime);
        glm::vec3 GetFirstColliderPosition();
    };

    

}