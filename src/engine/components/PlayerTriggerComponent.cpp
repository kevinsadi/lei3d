#include "PlayerTriggerComponent.hpp"
#include "components/StaticCollider.hpp"

namespace lei3d {
    PlayerTriggerComponent::PlayerTriggerComponent(Entity& entity)
    : Component(entity)
    {
        StaticCollider* colliderComponent = entity.GetComponent<StaticCollider>();
        if (colliderComponent != nullptr) {
            m_collider = colliderComponent->getRigidBody();
			m_collider->setUserPointer(static_cast<void*>(&m_callback)); 
        }
    }


	PlayerTriggerComponent::~PlayerTriggerComponent() {
		if (m_collider != nullptr)
			m_collider->setUserPointer(nullptr);
	}

	void PlayerTriggerComponent::SetCollider(btCollisionObject* collider) {
		if (m_collider != nullptr)
			m_collider->setUserPointer(nullptr);

		m_collider = collider;
		m_collider->setUserPointer(static_cast<void*>(&m_callback));
	}

	void PlayerTriggerComponent::SetOnPlayerEntered(const std::function<void()>& onPlayerEntered){
		m_callback.SetOnPlayerEntered(onPlayerEntered);	
	}

	void PlayerTriggerComponent::SetOnPlayerStayed(const std::function<void()>& onPlayerStayed){
		m_callback.SetOnPlayerStayed(onPlayerStayed);
	}

	
	void PlayerTriggerComponent::SetOnPlayerExited(const std::function<void()>& onPlayerExited) {
		m_callback.SetOnPlayerExited(onPlayerExited);	
	}
	
}
