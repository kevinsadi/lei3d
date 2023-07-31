#include "CharacterController.hpp"

namespace lei3d {
    // DEFINE_COMPONENT(Backpack, "Backpack");

    CharacterController::CharacterController(Entity* entity) : Component(entity) {

    }

    void CharacterController::Start() {
        LEI_TRACE("Character Controller started");
    }

    /**
     * @brief - Add a cylinder collision mesh to this object. DO THIS BEFORE MAKING ANY OTHER MESHES 
     * 
     */
    void CharacterController::Init() {
        //CHARACTER--------------------
        //std::unique_ptr<btCollisionShape> character = std::make_unique<btCapsuleShape>(btScalar{1.0f}, btScalar{3.0f});
        btCollisionShape* character = new btCapsuleShape(btScalar{1.0f}, btScalar{3.0f});
        btTransform startTransform;
        startTransform.setIdentity();

        btScalar mass{1.f};
        btVector3 localInertia{0.0f, 0.0f, 0.0f};
        character->calculateLocalInertia(mass, localInertia);
        Transform transform = m_Entity->m_Transform;
        startTransform.setOrigin(btVector3{transform.position.x, transform.position.y, transform.position.z});
        
        //THIS IS A MEMORY LEAK, FIX!!
        btDefaultMotionState* charMotionState = new btDefaultMotionState(startTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, charMotionState, character, localInertia);
        btRigidBody* characterBody = new btRigidBody(rbInfo);
        characterBody->setSleepingThresholds(0.0, 0.0);
        characterBody->setAngularFactor(0.0);
        ActiveScene().GetPhysicsWorld().m_dynamicsWorld->addRigidBody(characterBody);
        ActiveScene().GetPhysicsWorld().m_collisionShapes.push_back(character);
    }

    void CharacterController::PhysicsUpdate(float deltaTime)
    {
        m_Entity->m_Transform.position = ActiveScene().GetPhysicsWorld().GetFirstColliderPosition();
    }
}