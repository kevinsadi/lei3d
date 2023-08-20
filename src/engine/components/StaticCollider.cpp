#include "StaticCollider.hpp"

#include "core/SceneManager.hpp"
#include "logging/Log.hpp"

namespace lei3d
{
    StaticCollider::StaticCollider(Entity& entity) : Component(entity) {

    }

    /**
     * @brief - Instantiate Static Collider on an entity. If it has a model.  
     *
     * Adds collider mesh to the physics object of the current scene. 
     * 
     */
    void StaticCollider::Init()
    {
    }

    void StaticCollider::SetColliderToModel(const Model& model)
    {  
        std::vector<btTriangleMesh*> modelMeshes = model.GetCollisionMeshes();
        for (auto triMesh: modelMeshes)
        {
            AddCollisionsFromTriangleMesh(triMesh, m_Entity.m_Transform);
        }
    }

    /**
     * @brief Mutates values in PhysicsWorld to add the triMesh to the dynamicsWorldScene
     *  
     * NOTE: triMesh not passed by const bc btBvhTriangleMeshShape does n ot  take in const
     * 
     * @param triMesh
     * @param transform
     */
    void StaticCollider::AddCollisionsFromTriangleMesh(btTriangleMesh* triMesh, const Transform& transform)
    {
        btVector3 scaleVector{transform.scale.x, transform.scale.y, transform.scale.z};
        btBvhTriangleMeshShape* nonScaledMeshShape = new btBvhTriangleMeshShape(triMesh, true, true);
        btScaledBvhTriangleMeshShape* meshShape = new btScaledBvhTriangleMeshShape(nonScaledMeshShape, scaleVector);

        // now add this mesh to our physics world.
        SceneManager::ActiveScene().GetPhysicsWorld().m_collisionShapes.push_back(meshShape);

        btTransform meshTransform;
        meshTransform.setIdentity();
        meshTransform.setOrigin(btVector3{transform.position.x, transform.position.y, transform.position.z});

        // mesh environment collisions are static 
        btScalar meshMass = 0.0f;
        btVector3 meshLocalInertia{0.0f, 0.0f, 0.0f};

        btDefaultMotionState* myMotionState = new btDefaultMotionState(meshTransform);
        btRigidBody::btRigidBodyConstructionInfo rbMeshInfo{meshMass, myMotionState, meshShape, meshLocalInertia};
        btRigidBody* meshBody = new btRigidBody(rbMeshInfo);

        SceneManager::ActiveScene().GetPhysicsWorld().m_dynamicsWorld->addRigidBody(meshBody);
    }

}