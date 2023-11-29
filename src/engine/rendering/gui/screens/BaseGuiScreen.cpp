#include "BaseGuiScreen.hpp"

#include "GLFW/glfw3.h"
#include "rendering/gui/components/GuiRect.hpp"
#include "rendering/gui/components/GuiTextBox.hpp"

#include <sstream>

namespace lei3d
{
	void BaseGuiScreen::Init()
	{
		GuiScreen::Init();
		GuiTextBox* flowersFoundTextbox = new GuiTextBox(
			std::to_string(m_timer),
			GuiComponent::Anchor::TOP_LEFT,
			{ GuiComponent::Space::NORMALIZED, { 0, 0 } },
			{ GuiTextBox::LineHeightMetric::PT, 100 },
			{ 1, 1, 1, 1 },
			{ 0, 0, 0, 0 });
		m_flowersFoundId = flowersFoundTextbox->GetId();
		;

		AddComponent(flowersFoundTextbox);

		GuiTextBox* splashScreenButton = new GuiTextBox(
			std::to_string(m_timer),
			GuiComponent::Anchor::CENTER_TOP,
			{ GuiComponent::Space::NORMALIZED, { 0, 0 } },
			{ GuiTextBox::LineHeightMetric::PT, 100 },
			{ 1, 1, 1, 1 },
			{ 0, 0, 0, 0 });
		m_timerTextboxId = splashScreenButton->GetId();

		AddComponent(splashScreenButton);
	}

	BaseGuiScreen::BaseGuiScreen()
	{
	}

	BaseGuiScreen::~BaseGuiScreen()
	{
	}

	void BaseGuiScreen::Update(const glm::vec2& screenSize, const glm::vec2& mousePos)
	{
		GuiScreen::Update(screenSize, mousePos);

		GuiTextBox* flowersFoundTextbox = (GuiTextBox*)m_components[m_flowersFoundId];
		flowersFoundTextbox->SetText(std::to_string(static_cast<int>(m_flowersFound)) + " / 3");

		if (m_flowersFound != 3)
		{
			m_timer += Application::DeltaTime();
			GuiTextBox* timerTextbox = (GuiTextBox*)m_components[m_timerTextboxId];
			int intTimer = static_cast<int>(std::floor(m_timer));

			std::stringstream ss;
			
			//timerTextbox->SetText(std::format("{:01d}:{:02d}", intTimer / 60, intTimer % 60));
		}
	}

	void BaseGuiScreen::AddFlower()
	{
		m_flowersFound++;
	}

	void BaseGuiScreen::ResetFlowers()
	{
		m_flowersFound = 0;
	}

	void BaseGuiScreen::ResetTimer()
	{
		m_timer = 0;
	}
} // namespace lei3d
