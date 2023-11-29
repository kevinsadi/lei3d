#include "CreditsScreen.hpp"

#include "MainMenuScreen.hpp"
#include "logging/Log.hpp"
#include "rendering/buffers/Texture.hpp"
#include "rendering/gui/GuiManager.hpp"
#include "rendering/gui/components/GuiTextBox.hpp"

namespace lei3d
{
	int CreditsScreen::GetCreditsTexID()
	{
		static Texture splash = Texture("./data/textures/credits.png");
		return splash.id();
	}

	void CreditsScreen::Init()
	{
		GuiScreen::Init();

		AddComponent(new GuiRect(
			GuiComponent::Anchor::TOP_LEFT,
			{ GuiComponent::Space::NORMALIZED, { 0, 0 } },
			{ GuiComponent::Space::NORMALIZED, { 1, 1 } },
			{1, 1, 1, 1}
		));

		AddComponent(new GuiRect(
			GuiComponent::Anchor::TOP_LEFT,
			{ GuiComponent::Space::NORMALIZED, { 0, 0 } },
			{ GuiComponent::Space::NORMALIZED, { 1, 1 } },
			{1, 1, 1, 1},
			GetCreditsTexID()
		));

		AddComponent(new GuiTextBox(
			"x",
			GuiComponent::Anchor::TOP_LEFT,
			{ GuiComponent::Space::PIXELS, { 100, 100 } },
			{ GuiTextBox::LineHeightMetric::PT, 100 },
			{ 1, 0, 0, 1 },
			{ 0, 0, 0, 0 },
			[]() {
				GuiManager::Instance().QueueNextScreen(new MainMenuScreen());
			}
		));
	}

	CreditsScreen::CreditsScreen()
	{
		
	}

	CreditsScreen::~CreditsScreen()
	{

	}
}
