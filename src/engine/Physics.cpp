#include "physics/Physics.hpp"
#include <iostream>

namespace lei3d
{
    PhysicsObjects CreatePhysicsWorld()
    {
        // this is just the default setup, we can make this fancier if we want
        btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
        btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
        // good general purpose broadphase, we cna also use btAxis3Sweep
        btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
        btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();
        btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
        dynamicsWorld->setGravity(btVector3(0, -10, 0));

        // Create collisionShapes object to keep track of all the objects we want to collide over (this will eventually include the whole scene)
        btAlignedObjectArray<btCollisionShape*> collisionShapes;

        // Create a character capsule that obeys the laws of gravity.
        btCollisionShape* character = new btCapsuleShape(btScalar{1.0f}, btScalar{3.0f});
        collisionShapes.push_back(character);

        btTransform startTransform;
        startTransform.setIdentity();
        startTransform.setOrigin(btVector3{0.0f, 0.0f, 0.0f});

        btScalar mass{1.f};

        btVector3 localInertia{0.0f, 0.0f, 0.0f};
        character->calculateLocalInertia(mass, localInertia);
        
        btDefaultMotionState* charMotionState = new btDefaultMotionState(startTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, charMotionState, character, localInertia);
        btRigidBody* characterBody = new btRigidBody(rbInfo);

        characterBody->setSleepingThresholds(0.0, 0.0);
        characterBody->setAngularFactor(0.0);

        dynamicsWorld->addRigidBody(characterBody);

        // return our physics objects, will be modified before passed to physicsStep
        PhysicsObjects objects {
            collisionConfiguration, 
            dispatcher, 
            overlappingPairCache, 
            solver, 
            dynamicsWorld,           
            collisionShapes
        };

        return objects;
    }

    // This is so messed up, makes a copy of physicsObjects struct, but every thing in the struct is a pointer, so we modify the values of the struct
    void PhysicsStep(PhysicsObjects physicsObjects, float deltaTime)
    {
        physicsObjects.dynamicsWorld->stepSimulation(deltaTime, 10);

        // Move every object
		for (int j = physicsObjects.dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
		{
			btCollisionObject* obj = physicsObjects.dynamicsWorld->getCollisionObjectArray()[j];
			btRigidBody* body = btRigidBody::upcast(obj);
			btTransform trans;
			if (body && body->getMotionState())
			{
				body->getMotionState()->getWorldTransform(trans);
			}
			else
			{
				trans = obj->getWorldTransform();
			}
		}
    }

    glm::vec3 GetFirstColliderPosition(PhysicsObjects physicsObjects)
    {
        btCollisionObject* obj = physicsObjects.dynamicsWorld->getCollisionObjectArray()[0];
        btRigidBody* body = btRigidBody::upcast(obj);
        btTransform trans;
        if (body && body->getMotionState())
        {
            body->getMotionState()->getWorldTransform(trans);
        }
        else
        {
            trans = obj->getWorldTransform();
        }
        glm::vec3 position = glm::vec3(float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
        return position;
    }

    /**
     * @brief Mutates values in physicsObject to add the triMesh to the dynamicsWorldScene
     * 
     * @param physicsObjects 
     * @param triMesh 
     */
    void AddCollisionsFromTriangleMesh(PhysicsObjects physicsObjects, btTriangleMesh* triMesh, Transform transform)
    {
        btVector3 scaleVector{transform.scale.x, transform.scale.y, transform.scale.z};
        btBvhTriangleMeshShape* nonScaledMeshShape = new btBvhTriangleMeshShape(triMesh, true, true);
        btScaledBvhTriangleMeshShape* meshShape = new btScaledBvhTriangleMeshShape(nonScaledMeshShape, scaleVector);

        // Now make the ground 
        physicsObjects.collisionShapes.push_back(meshShape);
        
        btTransform meshTransform;
        meshTransform.setIdentity();
        meshTransform.setOrigin(btVector3{transform.position.x, transform.position.y, transform.position.z});
        //meshTransform.setOrigin(btVector3{0, 0, 0});

        // mesh environment collisions are static 
        btScalar meshMass = 0.0f;
        btVector3 meshLocalInertia{0.0f, 0.0f, 0.0f};
        
        btDefaultMotionState* myMotionState = new btDefaultMotionState(meshTransform);
        btRigidBody::btRigidBodyConstructionInfo rbMeshInfo{meshMass, myMotionState, meshShape, meshLocalInertia};
        btRigidBody* meshBody = new btRigidBody(rbMeshInfo);

        physicsObjects.dynamicsWorld->addRigidBody(meshBody);
    }


}