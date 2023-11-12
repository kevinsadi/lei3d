#include "BaseGuiScreen.hpp"

#include "GLFW/glfw3.h"
#include "rendering/gui/components/GuiRect.hpp"


namespace lei3d
{
	void BaseGuiScreen::Init()
	{
		GuiScreen::Init();

		GuiRect* colorBar = new GuiRect(
			GuiComponent::Anchor::TOP_LEFT, 
			{GuiComponent::Space::PIXELS, {50, 50}},
			{ GuiComponent::Space::PIXELS, { 100, 100 } }
		);
		m_colorBarId = colorBar->GetId();

		AddComponent(colorBar);
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
		GuiRect* colorBar = (GuiRect*)m_components[m_colorBarId];
		colorBar->SetColor(glm::vec4(
			0.5f + 0.5f * sinf((float)glfwGetTime()),
			0.5f + 0.5f * sinf((float)glfwGetTime() + 2.0f),
			0.5f + 0.5f * sinf((float)glfwGetTime() + 4.0f),
			1.0f
		));
	}
}
