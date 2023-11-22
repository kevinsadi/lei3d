#pragma once

#include "core/Component.hpp"

namespace lei3d
{
	class TimerComponent: public Component
	{
	private:
		float m_Timer = 0.0f;
		bool m_Start = false;
		float m_TargetTime = 0.0f;
		std::function<void()> m_Event;
	public:
		TimerComponent(Entity& entity);
		~TimerComponent() {}

		void StartTimer();
		void SetTargetTime(float time) { m_TargetTime = time; }
		void OnTimerEnd(const std::function<void()>& event) { m_Event = event; }
	private:
		void Update() override;
#ifdef EDITOR
		void OnImGuiRender() override;
#endif
	};
}
