#include "MainMenuScreen.hpp"

#include "rendering/gui/components/GuiTextBox.hpp"
#include "rendering/gui/components/GuiTextureRect.hpp"
#include "Logging/Log.hpp"
#include "rendering/buffers/Texture.hpp"
#include "rendering/gui/GuiManager.hpp"

namespace lei3d
{
	int MainMenuScreen::GetSplashTexId()
	{
		static Texture splash = Texture("./data/textures/splash.png");
		return splash.id();
	}

	MainMenuScreen::MainMenuScreen()
	{
		
	}

	MainMenuScreen::~MainMenuScreen()
	{
	}

	void MainMenuScreen::Init()
	{
		GuiScreen::Init();

		AddComponent(new GuiTextureRect(
			GetSplashTexId(),
			GuiComponent::Anchor::TOP_LEFT,
			{ GuiComponent::Space::NORMALIZED, { 0, 0 } },
			{ GuiComponent::Space::NORMALIZED, { 1, 1 } })
		);

		AddComponent(new GuiTextBox(
			"Click Me",
			GuiComponent::Anchor::CENTER,
			{ GuiComponent::Space::NORMALIZED, { 0, 0 } },
			[]() {
				LEI_INFO("Button Clicked, Closing GUI Screen.");
				GuiManager::Instance().CloseActiveScreen();
			})
		);
	}
} 