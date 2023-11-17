#include "BaseGuiScreen.hpp"

#include "GLFW/glfw3.h"
#include "rendering/gui/components/GuiRect.hpp"
#include "rendering/gui/components/GuiTextBox.hpp"

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

		// set color of color bar to be sin function of time (for testing)
		GuiTextBox* flowersFoundTextbox = (GuiTextBox*)m_components[m_flowersFoundId];
		flowersFoundTextbox->SetText(std::to_string(static_cast<int>(m_flowersFound)) + " / 3");

		m_timer += Application::DeltaTime();
		GuiTextBox* timerTextbox = (GuiTextBox*)m_components[m_timerTextboxId];
		timerTextbox->SetText(std::to_string(static_cast<int>(std::floor(m_timer))));
	}
} // namespace lei3d
