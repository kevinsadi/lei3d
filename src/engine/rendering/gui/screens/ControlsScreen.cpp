#include "ControlsScreen.hpp"

#include "MainMenuScreen.hpp"
#include "logging/Log.hpp"
#include "rendering/buffers/Texture.hpp"
#include "rendering/gui/GuiManager.hpp"
#include "rendering/gui/components/GuiTextBox.hpp"

namespace lei3d
{
	int ControlsScreen::GetControlsTexID()
	{
		static Texture splash = Texture("./data/textures/controls.png");
		return splash.id();
	}

	void ControlsScreen::Init()
	{
		GuiScreen::Init();

		AddComponent(new GuiRect(
			GuiComponent::Anchor::TOP_LEFT,
			{ GuiComponent::Space::NORMALIZED, { 0, 0 } },
			{ GuiComponent::Space::NORMALIZED, { 1, 1 } },
			{ 1, 1, 1, 1 }));

		AddComponent(new GuiRect(
			GuiComponent::Anchor::TOP_LEFT,
			{ GuiComponent::Space::NORMALIZED, { 0, 0 } },
			{ GuiComponent::Space::NORMALIZED, { 1, 1 } },
			{ 1, 1, 1, 1 },
			GetControlsTexID()));

		AddComponent(new GuiTextBox(
			"x",
			GuiComponent::Anchor::TOP_LEFT,
			{ GuiComponent::Space::PIXELS, { 100, 100 } },
			{ GuiTextBox::LineHeightMetric::PT, 100 },
			{ 1, 0, 0, 1 },
			{ 0, 0, 0, 0 },
			[]() {
				GuiManager::Instance().QueueNextScreen(new MainMenuScreen());
			}));
	}

	ControlsScreen::ControlsScreen()
	{
	}

	ControlsScreen::~ControlsScreen()
	{
	}
} // namespace lei3d
