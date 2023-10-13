#include "LevelSwitchCollider.hpp"
#include "core/SceneManager.hpp"

namespace lei3d
{
	LevelSwitchCollider::LevelSwitchCollider(Entity& entity)
		: TriggerCollider(entity)
	{
	}

	LevelSwitchCollider::~LevelSwitchCollider()
	{
	}

	void LevelSwitchCollider::OnTriggerStay(const btCollisionObject* other)
	{
		LEI_TRACE("YEAH YOU DID IT QUEEN, SWITCH LEVELS BABY");
		// SceneManager::SetScene("Test Kevin");
	}
} // namespace lei3d