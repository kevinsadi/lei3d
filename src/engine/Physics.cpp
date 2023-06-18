#include "include/physics/Physics.hpp"

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

        // Create collisionShapes object to keep track of all the objects we want to collide over (this will be the scene)
        btAlignedObjectArray<btCollisionShape*> collisionShapes;

        // Now we want to create our Rigid Bodies, starting with the ground
        btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));
        collisionShapes.push_back(groundShape);
        
        btTransform groundTransform;
        groundTransform.setIdentity();
        groundTransform.setOrigin(btVector3(0, -56, 0));

        btScalar floorMass = 0.0f;
        btVector3 floorLocalInertia{0.0f, 0.0f, 0.0f}; // list initialization makes my brain happy with structs. I'm sorry for the sudden switch haha
        
        btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
        btRigidBody::btRigidBodyConstructionInfo rbFloorInfo{floorMass, myMotionState, groundShape, floorLocalInertia};
        btRigidBody* floorBody = new btRigidBody(rbFloorInfo);

        dynamicsWorld->addRigidBody(floorBody);

        // now create a random character capsule that obeys the laws of gravity. Soon we will replace both of these with actual meshes
        btCollisionShape* character = new btCapsuleShape(btScalar{1.0f}, btScalar{3.0f});
        collisionShapes.push_back(character);

        btTransform startTransform;
        startTransform.setIdentity();

        btScalar mass{1.f};

        btVector3 localInertia{0.0f, 0.0f, 0.0f};
        character->calculateLocalInertia(mass, localInertia);
        startTransform.setOrigin(btVector3{3.0f, 10.0f, 0.0f});
        
        btDefaultMotionState* charMotionState = new btDefaultMotionState(startTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, charMotionState, character, localInertia);
        btRigidBody* characterBody = new btRigidBody(rbInfo);

        dynamicsWorld->addRigidBody(characterBody);
        
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

    void PhysicsStep(PhysicsObjects physicsObjects)
    {
        physicsObjects.dynamicsWorld->stepSimulation(1.0f / 60.0f, 10);

        //print positions of all objects
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
			//printf("world pos object %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
		}
    }


}