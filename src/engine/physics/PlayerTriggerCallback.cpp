#include "PlayerTriggerCallback.hpp"


namespace lei3d {
	PlayerTriggerCallback::PlayerTriggerCallback()
	{
		m_onPlayerEntered = [&](){};
		m_onPlayerStayed = [&](){};
		m_onPlayerExited = [&](){};
	}
} // namespace lei3d

