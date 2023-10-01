#pragma once

#include "core/Component.hpp"
#include <unordered_set>
#include <set>
#include <btBulletDynamicsCommon.h>

namespace lei3d
{
	/// <summary>
	/// Trigger Collider component
	/// </summary>
	class TriggerCollider : public Component
	{
	public:
		TriggerCollider(Entity& entity);
		~TriggerCollider();

		void Init(btCollisionObject* trigger, std::vector<const btCollisionObject*>& ignoredColliders);
		void Start() override;
		void Update() override;
		void PhysicsUpdate() override;
		void OnImGuiRender() override;
		void OnReset() override;


	protected:
		virtual void OnTriggerEnter(const btCollisionObject* enteredObject);
		virtual void OnTriggerStay(const btCollisionObject* other);
		virtual void OnTriggerExit(const btCollisionObject* exitedObjects);

	private:
		btCollisionObject* m_trigger;
		std::unordered_set<const btCollisionObject*> m_ignoredColliders;
		std::set<const btCollisionObject*> m_colliders; 
	};
} // namespace lei3d