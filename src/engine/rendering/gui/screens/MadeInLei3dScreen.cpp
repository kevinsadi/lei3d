#include "MadeInLei3dScreen.hpp"

#include "MainMenuScreen.hpp"
#include "core/SceneManager.hpp"
#include "logging/Log.hpp"
#include "rendering/buffers/Texture.hpp"
#include "rendering/gui/GuiManager.hpp"
#include "rendering/gui/components/GuiTextBox.hpp"
#include "rendering/gui/components/GuiRect.hpp"
#include "audio/AudioPlayer.hpp"

#include <time.h>

namespace lei3d
{
    // Copied from MainMenuScreen.cpp
    int MadeInLei3dScreen::GetLei3dTexture()
	{
		// image is 1725x407
		static Texture tex = Texture("./data/textures/lei3d.png", true);
		return tex.id();
	}

    void MadeInLei3dScreen::Init()
    {
        GuiScreen::Init();

        m_shouldHideHUD = true;

        GuiRect* lei3dlogo = new GuiRect(
			GuiComponent::Anchor::CENTER,
			{ GuiComponent::Space::NORMALIZED, { 0, 0 } },
			{ GuiComponent::Space::PIXELS, { 1725, 407 } },
			{ 1, 1, 1, 1 },
			GetLei3dTexture());
        lei3dlogo->m_alignCenter = true;

        AddComponent(lei3dlogo);

        start = clock();
    }

    void MadeInLei3dScreen::Update(const glm::vec2& screenSize, const glm::vec2& mousePos)
    {
        GuiScreen::Update(screenSize, mousePos);

        if (clock() - start > 3 * CLOCKS_PER_SEC)
        {
            GuiManager::Instance().QueueNextScreen(new MainMenuScreen());
        }
    }

    MadeInLei3dScreen::MadeInLei3dScreen() {

    }

    MadeInLei3dScreen::~MadeInLei3dScreen() {

    }

    void MadeInLei3dScreen::HandleInput() {

    }
} // namespace lei3d