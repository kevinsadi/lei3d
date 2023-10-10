#include "TriggerCollider.hpp"
#include "TriggerColliderCallback.hpp"
#include "core/SceneManager.hpp"


namespace lei3d {
	TriggerCollider::TriggerCollider(Entity& entity)
		: m_trigger(nullptr), Component(entity)
	{
	}

	TriggerCollider::~TriggerCollider()
	{
	}

	void TriggerCollider::Init(btCollisionObject* trigger, std::vector<const btCollisionObject*>& ignoredColliders)
	{
		m_trigger = trigger;
		for (auto* collider : ignoredColliders) {
			m_ignoredColliders.insert(collider);
		}
	}

	void TriggerCollider::Start()
	{
	}
	void TriggerCollider::Update()
	{
	}
	void TriggerCollider::PhysicsUpdate()
	{
		if (m_trigger == nullptr) {
			assert("Trigger Collider has not been initialized");
		}

		TriggerColliderCallback callback(&m_ignoredColliders);
		PhysicsWorld& world = SceneManager::ActiveScene().GetPhysicsWorld();
		world.m_dynamicsWorld
			->getCollisionWorld()
			->contactTest(m_trigger, std::ref(callback)); // Performs the contact test

		// This is O(4n) = O(n) so there's probably a better way to do this, but whatever 
		std::vector<const btCollisionObject*> justEneteredCols;
		std::vector<const btCollisionObject*> justExitedCols;
		for (auto* collider : callback.m_touchingColliders) {
			if (m_colliders.find(collider) != m_colliders.end()) {
				OnTriggerStay(collider);
			}
			else {
				justEneteredCols.push_back(collider);
				OnTriggerEnter(collider);
			}
		}

		for (auto* collider : m_colliders) {
			if (callback.m_touchingColliders.find(collider) == callback.m_touchingColliders.end())
			{
				justExitedCols.push_back(collider);
				OnTriggerExit(collider);
			}
		}

		for (auto* collider : justEneteredCols) {
			m_colliders.insert(collider);
		}

		for (auto* collider : justExitedCols) {
			m_colliders.erase(collider);
		}
	}
	void TriggerCollider::OnImGuiRender()
	{
	}
	void TriggerCollider::OnReset()
	{
	}
	void TriggerCollider::OnTriggerEnter(const btCollisionObject* enteredObject)
	{
#if _DEBUG
		//LEI_TRACE("ENTERED");
#endif
	}
	void TriggerCollider::OnTriggerStay(const btCollisionObject* other)
	{
#if _DEBUG
		//LEI_TRACE("STAY");
#endif
	}
	void TriggerCollider::OnTriggerExit(const btCollisionObject* exitedObjects)
	{
#if _DEBUG
		//LEI_TRACE("EXIT");
#endif
	}
} // namespace lei3d
