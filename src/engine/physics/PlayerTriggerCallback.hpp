#pragma once

#include <functional>

namespace lei3d {
	class PlayerTriggerCallback {
		public:
			PlayerTriggerCallback();

			// Called by the character controller
			void PlayerEntered() { m_onPlayerEntered(); };
			void PlayerStayed() { m_onPlayerExited(); };
			void PlayerExited() { m_onPlayerExited(); };


			void SetOnPlayerEntered(const std::function<void()>& onPlayerEntered) { m_onPlayerEntered = onPlayerEntered; };
			void SetOnPlayerStayed(const std::function<void()>& onPlayerStayed) { m_onPlayerStayed = onPlayerStayed; };
			void SetOnPlayerExited(const std::function<void()>& onPlayerExited) { m_onPlayerExited = onPlayerExited; };

		private:
			std::function<void()> m_onPlayerEntered;
			std::function<void()> m_onPlayerStayed;
			std::function<void()> m_onPlayerExited;
	};
}

