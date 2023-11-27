#include "LoadingScreen.hpp"

#include "core/SceneManager.hpp"
#include "GLFW/glfw3.h"
#include "rendering/gui/GuiManager.hpp"
#include "rendering/gui/components/GuiTextBox.hpp"

namespace lei3d
{
	LoadingScreen::LoadingScreen()
	{
	}

	LoadingScreen::~LoadingScreen()
	{
	}

	void LoadingScreen::HandleInput()
	{
		// don't close when they press escape
	}

	void LoadingScreen::Update(const glm::vec2& screenSize, const glm::vec2& mousePos)
	{
		static bool firstUpdate = true;

		GuiScreen::Update(screenSize, mousePos);

		if (firstUpdate)
		{
			firstUpdate = false;

			SceneManager::SetScene(1);
			GuiManager::Instance().QueueNextScreen(nullptr);
		}
	}

	void LoadingScreen::Init()
	{
		GuiScreen::Init();

		m_shouldHideHUD = true;

		GuiRect* background = new GuiRect(
			GuiComponent::Anchor::TOP_LEFT,
			{ GuiComponent::Space::NORMALIZED, { 0, 0 } },
			{ GuiComponent::Space::NORMALIZED, { 1, 1 } },
			{ 0.4, 0.4, 0.4, 0.7 });

		GuiTextBox* loading = new GuiTextBox(
			"Loading Game...",
			GuiComponent::Anchor::CENTER,
			{ GuiComponent::Space::NORMALIZED, { 0, 0 } },
			{ GuiTextBox::LineHeightMetric::PT, 100 },
			{ 0.827, 0.827, 0.827, 1 },
			{ 0, 0, 0, 0 });

		loading->m_alignCenter = true;

		AddComponent(background);
		AddComponent(loading);
	}
} // namespace lei3d
