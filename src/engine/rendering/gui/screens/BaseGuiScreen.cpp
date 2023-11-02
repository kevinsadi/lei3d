#include "BaseGuiScreen.hpp"

#include "rendering/gui/components/GuiTextBox.hpp"


namespace lei3d
{
	void BaseGuiScreen::Init()
	{
		GuiScreen::Init();

		GuiTextBox* text = new GuiTextBox("Hello");

		AddComponent(text);
	}

	BaseGuiScreen::BaseGuiScreen()
	{
		
	}

	BaseGuiScreen::~BaseGuiScreen()
	{
		
	}
}
