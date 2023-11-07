#include "BaseGuiScreen.hpp"

#include "rendering/gui/components/GuiTextBox.hpp"


namespace lei3d
{
	void BaseGuiScreen::Init()
	{
		GuiScreen::Init();

		GuiTextBox* text = new GuiTextBox("Put HUD components here.", GuiComponent::Anchor::TOP_LEFT, {GuiComponent::Space::NORMALIZED, {0.1, 0.1}});

		AddComponent(text);
	}

	BaseGuiScreen::BaseGuiScreen()
	{
		
	}

	BaseGuiScreen::~BaseGuiScreen()
	{
		
	}
}
