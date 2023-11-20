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

		/// <summary>
		/// Reassigns the trigger to an arbitrary one, and sets the ignored colliders
		/// </summary>
		/// <param name="trigger"> The collider to attatch to this trigger </param>
		/// <param name="ignoredColliders"> The list of ignored colliders </param>
		void Init(btCollisionObject* trigger, std::vector<const btCollisionObject*>& ignoredColliders);

		/// <summary>
		/// Inits the ignored colliders of this trigger object
		/// </summary>
		/// <param name="ignoredColliders"> The list of ignored colliders </param>
		void Init(std::vector<const btCollisionObject*>& ignoredColliders);

		/// <summary>
		/// Adds another collider to the list of ignored colliders
		/// </summary>
		/// <param name="ignoredCollider"> The collider to add </param>
		void AddIgnoredCollider(const btCollisionObject* ignoredCollider);

		/// <summary>
		/// Removes a collider from the list of ignored colliders
		/// </summary>
		/// <param name="ignoredCollider"> The collider to remove </param>
		void RemoveIgnoredCollider(const btCollisionObject* ignoredCollider);

		void Start() override;
		void Update() override;
		void PhysicsUpdate() override;
#ifdef EDITOR
		void OnImGuiRender() override;
#endif
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