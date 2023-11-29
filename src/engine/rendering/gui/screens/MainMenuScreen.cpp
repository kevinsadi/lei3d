#include "MainMenuScreen.hpp"

#include "ControlsScreen.hpp"
#include "CreditsScreen.hpp"
#include "LoadingScreen.hpp"
#include "core/SceneManager.hpp"
#include "logging/Log.hpp"
#include "rendering/buffers/Texture.hpp"
#include "rendering/gui/GuiManager.hpp"
#include "rendering/gui/components/GuiTextBox.hpp"
#include "rendering/gui/components/GuiRect.hpp"
#include "audio/AudioPlayer.hpp"

namespace lei3d
{
	int MainMenuScreen::GetLei3dTexture()
	{
		// image is 1725x407
		static Texture tex = Texture("./data/textures/lei3d.png", true);
		return tex.id();
	}

	int MainMenuScreen::GetSkyLeiTexture()
	{
		// image is 466x165
		static Texture tex = Texture("./data/textures/SkyLei.png", true);
		return tex.id();
	}

	void MainMenuScreen::Init()
	{
		GuiScreen::Init();

		m_shouldHideHUD = true;

		GuiRect* skyleilogo = new GuiRect(
			GuiComponent::Anchor::CENTER,
			{ GuiComponent::Space::NORMALIZED, { 0.025, -.2 } },
			{ GuiComponent::Space::PIXELS, { 699, 247.5 } },
			{ 1, 1, 1, 1 },
			GetSkyLeiTexture());
		skyleilogo->m_alignCenter = true;

		GuiRect* lei3dlogo = new GuiRect(
			GuiComponent::Anchor::BOTTOM_RIGHT,
			{ GuiComponent::Space::PIXELS, { -800, -250 } },
			{ GuiComponent::Space::PIXELS, { 862.5, 203.5 } },
			{ 1, 1, 1, 1 },
			GetLei3dTexture());

		GuiTextBox* startGame = new GuiTextBox(
			"START",
			GuiComponent::Anchor::CENTER,
			{ GuiComponent::Space::NORMALIZED, { 0, 0.1 } },
			{ GuiTextBox::LineHeightMetric::PT, 100 },
			{ 0.827, 0.827, 0.827, 1 },
			{ 0, 0, 0, 0 },
			[]() {
				AudioPlayer::GetInstance().PlaySFX("menu_reverse.mp3");
				LEI_INFO("Starting game.");
				GuiManager::Instance().QueueNextScreen(new LoadingScreen());
			});

		startGame->SetOnHover([this, startGame]() {
			if (!m_enteredHover)
			{
				AudioPlayer::GetInstance().PlaySFX("menu_select.mp3");
				m_enteredHover = true;
			}
			((GuiTextBox*)(this->m_components[startGame->GetId()]))->SetTextColor({ 0.522, 0.827, 0.965, 1 });
		});
		startGame->SetOnStopHover([this, startGame]() {
			m_enteredHover = false;
			((GuiTextBox*)(this->m_components[startGame->GetId()]))->SetTextColor({ 0.827, 0.827, 0.827, 1 });
		});

		startGame->m_alignCenter = true;

		GuiTextBox* creditsButton = new GuiTextBox(
			"CREDITS",
			GuiComponent::Anchor::CENTER,
			{ GuiComponent::Space::NORMALIZED, { 0, 0.2 } },
			{ GuiTextBox::LineHeightMetric::PT, 50 },
			{ 0.827, 0.827, 0.827, 1 },
			{ 0, 0, 0, 0 },
			[]() {
				GuiManager::Instance().QueueNextScreen(new CreditsScreen());
			});

		creditsButton->SetOnHover([this, creditsButton]() {
			if (!m_enteredHover)
			{
				AudioPlayer::GetInstance().PlaySFX("menu_select.mp3");
				m_enteredHover = true;
			}
			((GuiTextBox*)(this->m_components[creditsButton->GetId()]))->SetTextColor({ 0.522, 0.827, 0.965, 1 });
		});
		creditsButton->SetOnStopHover([this, creditsButton]() {
			m_enteredHover = false;
			((GuiTextBox*)(this->m_components[creditsButton->GetId()]))->SetTextColor({ 0.827, 0.827, 0.827, 1 });
		});

		creditsButton->m_alignCenter = true;

		GuiTextBox* controlsButton = new GuiTextBox(
			"CONTROLS",
			GuiComponent::Anchor::CENTER,
			{ GuiComponent::Space::NORMALIZED, { 0, 0.3 } },
			{ GuiTextBox::LineHeightMetric::PT, 50 },
			{ 0.827, 0.827, 0.827, 1 },
			{ 0, 0, 0, 0 },
			[]() {
				GuiManager::Instance().QueueNextScreen(new ControlsScreen());
			});

		controlsButton->SetOnHover([this, controlsButton]() {
			if (!m_enteredHover)
			{
				AudioPlayer::GetInstance().PlaySFX("menu_select.mp3");
				m_enteredHover = true;
			}
			((GuiTextBox*)(this->m_components[controlsButton->GetId()]))->SetTextColor({ 0.522, 0.827, 0.965, 1 });
		});
		controlsButton->SetOnStopHover([this, controlsButton]() {
			m_enteredHover = false;
			((GuiTextBox*)(this->m_components[controlsButton->GetId()]))->SetTextColor({ 0.827, 0.827, 0.827, 1 });
		});

		controlsButton->m_alignCenter = true;

		AddComponent(skyleilogo);
		AddComponent(lei3dlogo);
		AddComponent(startGame);
		AddComponent(creditsButton);
		AddComponent(controlsButton);
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
} // namespace lei3d
