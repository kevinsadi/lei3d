#include "StaticCollider.hpp"
#include "logging/Log.hpp"

namespace lei3d
{
    StaticCollider::StaticCollider(Entity* entity) : Component(entity) {

    }

    /**
     * @brief - Instantiate Static Collider on an entity. If it has a model.  
     *
     * Adds collider mesh to the physics object of the current scene. 
     * 
     */
    void StaticCollider::Init()
    {
        GiveModelStaticCollider();
    }

    void StaticCollider::GiveModelStaticCollider()
    {
        Model* entityModel = m_Entity->GetComponent<Model>();
        
        if (entityModel)
        {
            std::vector<btTriangleMesh*> modelMeshes = entityModel->GetCollisionMeshesFromModel();
            for (auto triMesh: modelMeshes)
            {
                AddCollisionsFromTriangleMesh(triMesh, m_Entity->m_Transform);
            }
        }
    }


    /**
     * @brief Mutates values in PhysicsWorld to add the triMesh to the dynamicsWorldScene
     *  
     * @param triMesh
     * @param transform
     */
    void StaticCollider::AddCollisionsFromTriangleMesh(btTriangleMesh* triMesh, Transform transform)
    {
        btVector3 scaleVector{transform.scale.x, transform.scale.y, transform.scale.z};
        btBvhTriangleMeshShape* nonScaledMeshShape = new btBvhTriangleMeshShape(triMesh, true, true);
        btScaledBvhTriangleMeshShape* meshShape = new btScaledBvhTriangleMeshShape(nonScaledMeshShape, scaleVector);

        // now add this mesh to our physics world.
        ActiveScene().GetPhysicsWorld().m_collisionShapes.push_back(meshShape);

        btTransform meshTransform;
        meshTransform.setIdentity();
        meshTransform.setOrigin(btVector3{transform.position.x, transform.position.y, transform.position.z});

        // mesh environment collisions are static 
        btScalar meshMass = 0.0f;
        btVector3 meshLocalInertia{0.0f, 0.0f, 0.0f};

        btDefaultMotionState* myMotionState = new btDefaultMotionState(meshTransform);
        btRigidBody::btRigidBodyConstructionInfo rbMeshInfo{meshMass, myMotionState, meshShape, meshLocalInertia};
        btRigidBody* meshBody = new btRigidBody(rbMeshInfo);

        ActiveScene().GetPhysicsWorld().m_dynamicsWorld->addRigidBody(meshBody);
    }

}