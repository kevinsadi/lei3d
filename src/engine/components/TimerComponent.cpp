#include "TimerComponent.hpp"

#include "core/Application.hpp"
#include <imgui.h>

namespace lei3d
{
	TimerComponent::TimerComponent(Entity& entity)
		: Component(entity)
	{
		m_Event = [&]() {};
	}

	void TimerComponent::StartTimer()
	{
		m_Start = true;
	}

	void TimerComponent::Update()
	{
		if (m_Start)
		{
			m_Timer += Application::DeltaTime();
			if (m_Timer >= m_TargetTime) {
				// Trigger an event / do something
				m_Event();
				m_Start = false;
			}
		}
	}

	void TimerComponent::OnImGuiRender()
	{
		if (ImGui::CollapsingHeader("Timer"))
		{
			ImGui::DragFloat("Target Time (sec.)", &m_TargetTime, 0.1f, 0.0f, 1000.0f, "%.2f");
			ImGui::Text("Started: %s", m_Start ? "True" : "False");
		}
	}
}