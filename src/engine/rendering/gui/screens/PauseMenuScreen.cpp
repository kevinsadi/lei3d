#include "PauseMenuScreen.hpp"

#include "SplashScreen.hpp"
#include "rendering/gui/components/GuiTextBox.hpp"
#include "Logging/Log.hpp"
#include "rendering/gui/GuiManager.hpp"

namespace lei3d
{
	PauseMenuScreen::PauseMenuScreen()
	{
		
	}

	PauseMenuScreen::~PauseMenuScreen()
	{
	}

	void PauseMenuScreen::Init()
	{
		GuiScreen::Init();

		AddComponent(new GuiRect(
			GuiComponent::Anchor::TOP_LEFT,
			{ GuiComponent::Space::NORMALIZED, { 0, 0 } },
			{ GuiComponent::Space::NORMALIZED, { 1, 1 } },
			{0, 0, 0, 0.5}
		));

		GuiTextBox* closeButton = new GuiTextBox(
			"Close",
			GuiComponent::Anchor::TOP_LEFT,
			{ GuiComponent::Space::PIXELS, { 50, 50 } },
			{ GuiTextBox::LineHeightMetric::PT, 100 },
			{0, 1, 1, 1},
			{ 0.827, 0.827, 0.827, 0.5 },
			[]() {
				LEI_INFO("Button Clicked, Closing GUI Screen.");
				GuiManager::Instance().CloseActiveScreen();
			}
		);

		int closeButtonId = closeButton->GetId();
		closeButton->SetOnHover([this, closeButtonId]() {
			((GuiTextBox*)(this->m_components[closeButtonId]))->SetTextColor({ 1, 0, 0, 1 });
		});
		closeButton->SetOnStopHover([this, closeButtonId]() {
			((GuiTextBox*)(this->m_components[closeButtonId]))->SetTextColor({ 0, 1, 1, 1 });
		});

		AddComponent(closeButton);

		GuiTextBox* splashScreenButton  = new GuiTextBox(
			"CLICK ME",
			GuiComponent::Anchor::CENTER,
			{ GuiComponent::Space::NORMALIZED, { 0, 0 } },
			{ GuiTextBox::LineHeightMetric::PT, 100 },
			{ 0, 1, 1, 1 },
			{ 0, 0, 0, 0 },
			[]() {
				LEI_INFO("Button Clicked, Opening Splash Screen.");
				GuiManager::Instance().QueueNextScreen(new SplashScreen());
			}
		);

		int splashScreenButttonId = splashScreenButton->GetId();
		splashScreenButton->SetOnHover([this, splashScreenButttonId]() {
			((GuiTextBox*)(this->m_components[splashScreenButttonId]))->SetTextColor({ 1, 0, 0, 1 });
		});
		splashScreenButton->SetOnStopHover([this, splashScreenButttonId]() {
			((GuiTextBox*)(this->m_components[splashScreenButttonId]))->SetTextColor({ 0, 1, 1, 1 });
		});

		AddComponent(splashScreenButton);
	}
} 