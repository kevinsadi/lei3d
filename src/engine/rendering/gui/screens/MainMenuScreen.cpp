#include "MainMenuScreen.hpp"

#include "core/SceneManager.hpp"
#include "logging/Log.hpp"
#include "rendering/gui/GuiManager.hpp"
#include "rendering/gui/components/GuiTextBox.hpp"

namespace lei3d
{
	void MainMenuScreen::Init()
	{
		GuiScreen::Init();

		m_shouldHideHUD = true;

		GuiTextBox* splashText = new GuiTextBox(
			"SkyLei",
			GuiComponent::Anchor::CENTER,
			{ GuiComponent::Space::NORMALIZED, { 0, -.1 } },
			{ GuiTextBox::LineHeightMetric::PT, 200 },
			{ 0.827, 0.827, 0.827, 1 },
			{ 0, 0, 0, 0 }
		);

		splashText->m_alignCenter = true;

		GuiTextBox* startGame = new GuiTextBox(
			"START",
			GuiComponent::Anchor::CENTER,
			{ GuiComponent::Space::NORMALIZED, { 0, 0.1 } },
			{ GuiTextBox::LineHeightMetric::PT, 100 },
			{ 0.827, 0.827, 0.827, 1 },
			{ 0, 0, 0, 0 },
			[]() {
				LEI_INFO("Starting game.");
				GuiManager::Instance().CloseActiveScreen();
				SceneManager::SetScene(1);
			}
		);

		startGame->SetOnHover([this, startGame]() {
			((GuiTextBox*)(this->m_components[startGame->GetId()]))->SetTextColor({ 0.353, 0.353, 0.353, 1 });
		});
		startGame->SetOnStopHover([this, startGame]() {
			((GuiTextBox*)(this->m_components[startGame->GetId()]))->SetTextColor({ 0.827, 0.827, 0.827, 1 });
		});

		startGame->m_alignCenter = true;

		AddComponent(splashText);
		AddComponent(startGame);
	}

	MainMenuScreen::MainMenuScreen()
	{
	}

	MainMenuScreen::~MainMenuScreen()
	{
	}

	void MainMenuScreen::HandleInput()
	{
		// don't want to close screen if they press escape
	}
}
