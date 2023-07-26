// no need for a class, at least right now
// nice to finally get some code going again
#ifndef PHYSICS
#define PHYSICS

#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionShapes/btScaledBvhTriangleMeshShape.h"

#include <stdio.h>
#include <glm/glm.hpp>

#include "util/Transform.hpp"

namespace lei3d
{
    struct PhysicsObjects
    {
        btDefaultCollisionConfiguration* collisionConfiguration = nullptr;
        btCollisionDispatcher* dispatcher = nullptr;
        btBroadphaseInterface* overlappingPairCache = nullptr;
        btSequentialImpulseConstraintSolver* solver = nullptr;
        btDiscreteDynamicsWorld* dynamicsWorld = nullptr;
        btAlignedObjectArray<btCollisionShape*> collisionShapes;
    };

    PhysicsObjects CreatePhysicsWorld();
    void PhysicsStep(PhysicsObjects physicsObjects, float deltaTime);
    glm::vec3 GetFirstColliderPosition(PhysicsObjects physicsObjects);
    void AddCollisionsFromTriangleMesh(PhysicsObjects physicsObjects, btTriangleMesh* triMesh, Transform transform);
}

#endif // PHYSICS