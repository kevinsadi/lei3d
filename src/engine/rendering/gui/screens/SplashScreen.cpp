#include "SplashScreen.hpp"

#include "logging/Log.hpp"
#include "rendering/buffers/Texture.hpp"
#include "rendering/gui/GuiManager.hpp"
#include "rendering/gui/components/GuiTextBox.hpp"

namespace lei3d
{
	int SplashScreen::GetSplashTexId()
	{
		static Texture splash = Texture("./data/textures/splash.png");
		return splash.id();
	}

	void SplashScreen::Init()
	{
		GuiScreen::Init();

		AddComponent(new GuiRect(
			GuiComponent::Anchor::TOP_LEFT,
			{ GuiComponent::Space::NORMALIZED, { 0, 0 } },
			{ GuiComponent::Space::NORMALIZED, { 1, 1 } },
			{1, 1, 1, 1},
			GetSplashTexId()
		));

		AddComponent(new GuiTextBox(
			"x",
			GuiComponent::Anchor::TOP_LEFT,
			{ GuiComponent::Space::PIXELS, { 100, 100 } },
			{ GuiTextBox::LineHeightMetric::PT, 100 },
			{ 1, 0, 0, 1 },
			{ 0, 0, 0, 0 },
			[]() {
				LEI_INFO("Button Clicked, Closing GUI Screen.");
				GuiManager::Instance().CloseActiveScreen();
			}
		));
	}

	SplashScreen::SplashScreen()
	{
		
	}

	SplashScreen::~SplashScreen()
	{

	}
}
