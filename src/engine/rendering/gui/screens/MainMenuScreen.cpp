#include "MainMenuScreen.hpp"

#include "SplashScreen.hpp"
#include "rendering/gui/components/GuiTextBox.hpp"
#include "rendering/gui/components/GuiTextureRect.hpp"
#include "Logging/Log.hpp"
#include "rendering/buffers/Texture.hpp"
#include "rendering/gui/GuiManager.hpp"

namespace lei3d
{
	MainMenuScreen::MainMenuScreen()
	{
		
	}

	MainMenuScreen::~MainMenuScreen()
	{
	}

	void MainMenuScreen::Update(const glm::vec2& screenSize, const glm::vec2& mousePos)
	{
		GuiScreen::Update(screenSize, mousePos);

		// set text color to red if mouse is over button
		if (m_components[m_closeButtonId]->GetMouseOver())
		{
			((GuiTextBox*)m_components[m_closeButtonId])->SetTextColor({ 1, 0, 0, 1 });
		}
		else
		{
			((GuiTextBox*)m_components[m_closeButtonId])->SetTextColor({ 0, 1, 1, 1 });
		}
	}

	void MainMenuScreen::Init()
	{
		GuiScreen::Init();

		AddComponent(new GuiRect(
			GuiComponent::Anchor::TOP_LEFT,
			{ GuiComponent::Space::NORMALIZED, { 0, 0 } },
			{ GuiComponent::Space::NORMALIZED, { 1, 1 } },
			nullptr,
			{0, 0, 0, 0.5}
		));

		GuiTextBox* closeButton = new GuiTextBox(
			"Close",
			GuiComponent::Anchor::TOP_LEFT,
			{ GuiComponent::Space::PIXELS, { 50, 50 } },
			[]() {
				LEI_INFO("Button Clicked, Closing GUI Screen.");
				GuiManager::Instance().CloseActiveScreen();
			},
			{ GuiTextBox::LineHeightMetric::PT, 100 },
			{ 0, 1, 1, 1 },
			{ 0.827, 0.827, 0.827, 0.5 }
		);
		m_closeButtonId = closeButton->GetId();
		AddComponent(closeButton);

		AddComponent(new GuiTextBox(
			"CLICK ME",
			GuiComponent::Anchor::CENTER,
			{ GuiComponent::Space::NORMALIZED, { 0, 0 } },
			[]() {
				LEI_INFO("Button Clicked, Opening Splash Screen.");
				GuiManager::Instance().QueueNextScreen(new SplashScreen());
			},
			{ GuiTextBox::LineHeightMetric::PT, 100 },
			{ 0, 1, 1, 1 },
			{ 0, 0, 0, 0 },
			false
		));
	}
} 