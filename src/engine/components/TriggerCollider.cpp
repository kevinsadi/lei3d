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

		#if _DEBUG
		size_t colCount = callback.m_touchingColliders.size();
		if (colCount != 0) {
			LEI_TRACE("Collided");
		}
		#endif

	}
	void TriggerCollider::OnImGuiRender()
	{
	}
	void TriggerCollider::OnReset()
	{
	}
} // namespace lei3d
