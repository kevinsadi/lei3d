#pragma once

#include "TriggerCollider.hpp"

namespace lei3d
{
	class LevelSwitchCollider : public TriggerCollider
	{
	public:
		LevelSwitchCollider(Entity& entity);
		~LevelSwitchCollider();

		void OnTriggerStay(const btCollisionObject* other) override;
	};
} // namespace lei3d