#include "CharacterController.hpp"

#include "core/SceneManager.hpp"
#include "components/FollowCameraController.hpp"

#include "util/BulletUtil.hpp"
#include <imgui.h>

#include "audio/AudioPlayer.hpp"

namespace lei3d
{
	CharacterController::CharacterController(Entity& entity)
		: Component(entity)
	{
	}

	CharacterController::~CharacterController()
	{
		delete m_Collider;
		delete m_MotionState;
		delete m_RigidBody;
		delete m_CharacterPhysicsUpdate;

		delete m_GroundCheckCollider;
		delete m_GroundCheckObj;
	}

	void CharacterController::Start()
	{
		LEI_TRACE("Character Controller started");

		// Add player back into the world if they are not in it.
		if (!m_IsInDynamicsWorld)
		{
			PhysicsWorld& world = SceneManager::ActiveScene().GetPhysicsWorld();
			world.m_dynamicsWorld->addRigidBody(m_RigidBody);
			m_IsInDynamicsWorld = true;
		}

		// Set the player's physics transform to the entity transform.
		btTransform trans;
		trans.setIdentity();
		trans.setOrigin(glmToBTVec3(m_Entity.m_Transform.position));

		// Reset Rigidbody & Ground Check
		m_RigidBody->setWorldTransform(trans);
		m_RigidBody->setLinearVelocity({ 0.0f, 0.0f, 0.0f });
		m_GroundCheckObj->setWorldTransform(getGroundCheckTransform(trans));
	}

	/**
	 * Add a cylinder collision mesh to this object. DO THIS BEFORE MAKING ANY OTHER MESHES.
	 * Also cannot change the size of the graphics mesh of the character controller for proper collisions.
	 *
	 */
	void CharacterController::Init(float width, float height, float groundCheckDist)
	{
		// CHARACTER--------------------
		m_Width = width;
		m_Height = height;
		m_GroundCheckDist = groundCheckDist;
		m_GroundCheckLocalPos = btVector3(0.f, -height, 0.f);

		m_Collider = new btCapsuleShape(btScalar{ width }, btScalar{ height });
		m_Collider->setMargin(0.0);

		btScalar mass{ 10.f };
		btVector3 localInertia{ 0.0f, 0.0f, 0.0f };
		m_Collider->calculateLocalInertia(mass, localInertia);

		btTransform startTransform = m_Entity.getBTTransform();
		m_MotionState = new btDefaultMotionState(startTransform);

		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, m_MotionState, m_Collider, localInertia);
		m_RigidBody = new btRigidBody(rbInfo);
		m_RigidBody->setSleepingThresholds(0.0, 0.0);
		m_RigidBody->setAngularFactor(0.0);
		m_RigidBody->setFriction(0.0);
		m_RigidBody->setRestitution(0.0);
		// Enable CCD for a rigid body
		m_RigidBody->setCcdMotionThreshold(0.0001); // Adjust the threshold as needed
		m_RigidBody->setCcdSweptSphereRadius(0.4);	// Adjust the swept sphere radius

		PhysicsWorld& world = SceneManager::ActiveScene().GetPhysicsWorld();
		world.m_dynamicsWorld->addRigidBody(m_RigidBody);
		// world.m_collisionShapes.push_back(m_Collider);

		// GROUND CHECK ------------------
		btTransform groundCheckTrans = getGroundCheckTransform(startTransform);

		m_GroundCheckCollider = new btSphereShape(m_GroundCheckDist);
		m_GroundCheckCollider->setMargin(0.0);
		m_GroundCheckObj = new btCollisionObject();
		m_GroundCheckObj->setCollisionShape(m_GroundCheckCollider);
		m_GroundCheckObj->setWorldTransform(groundCheckTrans);

		// WITHIN THIS CUSTOM PHYSICS UPDATE IS THE MAGIC THAT MAKES AIRSTRAFING / SURF POSSIBLE
		m_CharacterPhysicsUpdate = new CharacterPhysicsUpdate(*this, m_RigidBody, m_GroundCheckObj, m_GroundCheckDist);
		world.m_dynamicsWorld->addAction(m_CharacterPhysicsUpdate);
	}

	void CharacterController::Update()
	{
		const FollowCameraController* cameraController = m_Entity.GetComponent<FollowCameraController>();
		if (cameraController != nullptr)
		{
			m_Entity.m_Transform.yawRotation = cameraController->GetCamera()->GetYaw();
		}

		if (m_Entity.m_Transform.position.y < m_deathPlaneY)
		{
			// LEI_INFO("Player Below Death Plane");

			// We probably want to split things up once we get a gui in.
			SceneManager::ActiveScene().Reset();
			SceneManager::ActiveScene().Play();
		}
	}

	void CharacterController::PhysicsUpdate()
	{
		btTransform characterTrans;
		// if entity pos changed
		if (m_Entity.m_ResetTransform)
		{
			m_Entity.m_ResetTransform = false;
			characterTrans = m_Entity.getBTTransform();
			m_RigidBody->setWorldTransform(characterTrans);
			m_MotionState->setWorldTransform(characterTrans);
		}
		else
		{
			m_MotionState->getWorldTransform(characterTrans);
		}
		m_GroundCheckObj->setWorldTransform(getGroundCheckTransform(characterTrans));
		m_Entity.setFromBTTransform(characterTrans);
	}

	void CharacterController::OnReset()
	{
		// Remove player from the world first
		PhysicsWorld& world = SceneManager::ActiveScene().GetPhysicsWorld();
		world.m_dynamicsWorld->removeRigidBody(m_RigidBody);
		m_IsInDynamicsWorld = false;
	}

	btTransform CharacterController::getGroundCheckTransform(const btTransform& parentTransform)
	{
		btTransform groundCheckTrans;
		groundCheckTrans.setIdentity();
		groundCheckTrans.setOrigin(m_GroundCheckLocalPos);
		groundCheckTrans.mult(parentTransform, groundCheckTrans);

		return groundCheckTrans;
	}

	bool CharacterController::IsGrounded() const
	{
		return m_Grounded;
	}

	btCollisionObject* CharacterController::getGroundCheckObj()
	{
		return this->m_CharacterPhysicsUpdate->getGroundCheckObj();
	}

	btRigidBody* CharacterController::getRigidBody()
	{
		return m_CharacterPhysicsUpdate->getRigidBody();
	}

	void CharacterController::OnImGuiRender()
	{
		if (ImGui::CollapsingHeader("Character Controller"))
		{
			ImGui::Text("Grounded: %s", m_Grounded ? "True" : "False");

			ImGui::InputFloat("Ground Acceleration", &m_accel);
			ImGui::InputFloat("Air Acceleration", &m_airAccel);
			ImGui::InputFloat("Max Speed", &m_maxSpeed);
			ImGui::InputFloat("Max Air Speed", &m_maxAirSpeed);

			ImGui::InputFloat("Friction", &m_friction);
			// ImGui::InputFloat("Air Friction", &m_airFriction);

			ImGui::InputFloat("Jump Power", &m_jumpPower);
			// ImGui::InputFloat("Jump Height", &m_jumpHeight);

			ImGui::InputFloat("Death Plane Y", &m_deathPlaneY);
		}
	}

} // namespace lei3d
