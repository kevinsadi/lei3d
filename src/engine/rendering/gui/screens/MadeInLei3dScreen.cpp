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

    // Copied from SplashScreen.cpp
    int MadeInLei3dScreen::GetSplashTexId()
	{
		static Texture splash = Texture("./data/textures/splash.png");
		return splash.id();
	}

    void MadeInLei3dScreen::Init()
    {
        GuiScreen::Init();

        alpha = 0.0;

        fadeState = INCREASING;

        m_shouldHideHUD = true;

        GuiRect* background = new GuiRect(
            GuiComponent::Anchor::TOP_LEFT,
            { GuiComponent::Space::NORMALIZED, { 0, 0 } },
            { GuiComponent::Space::NORMALIZED, { 1, 1 } },
            { 1, 1, 1, 1 },
            GetSplashTexId()
        );

        GuiRect* lei3dlogo = new GuiRect(
			GuiComponent::Anchor::CENTER,
			{ GuiComponent::Space::NORMALIZED, { 0, -0.2 } },
			{ GuiComponent::Space::PIXELS, { 1725, 407 } },
			{ 1, 1, 1, alpha },
			GetLei3dTexture());
        lei3dlogo->m_alignCenter = true;

        logo_id = lei3dlogo->GetId();

        AddComponent(background);
        AddComponent(lei3dlogo);

        start = clock();
    }

    void MadeInLei3dScreen::Update(const glm::vec2& screenSize, const glm::vec2& mousePos)
    {
        GuiScreen::Update(screenSize, mousePos);

        delete m_components[logo_id];

        UpdateAlpha();

        GuiRect* lei3dlogo = new GuiRect(
			GuiComponent::Anchor::CENTER,
			{ GuiComponent::Space::NORMALIZED, { 0, -0.2 } },
			{ GuiComponent::Space::PIXELS, { 1725, 407 } },
			{ 1, 1, 1, alpha },
			GetLei3dTexture());
        lei3dlogo->m_alignCenter = true;
        
        m_components[logo_id] = lei3dlogo;

        if (clock() - start > 4 * CLOCKS_PER_SEC)
        {
            GuiManager::Instance().QueueNextScreen(new MainMenuScreen());
        }
    }

    /**
     * Make the fade-in and fade-out
     */
    void MadeInLei3dScreen::UpdateAlpha() {
        switch (fadeState) {
            case INCREASING:
                if (alpha < 1) {
                    alpha += 0.03;
                } else {
                    fadeState = STATIC;
                }
                break;
            case STATIC:
                // 1284 for delta_alpha = 0.02, 897 for 0.03
                if ((4 * CLOCKS_PER_SEC) - (clock() - start) <= 897) { // 897 = number of clocks to get alpha >= 1
                    fadeState = DECREASING;
                }
                break;
            case DECREASING:
                alpha -= 0.03;
                break;
            default:
                break;
        }
    }

    MadeInLei3dScreen::MadeInLei3dScreen() {

    }

    MadeInLei3dScreen::~MadeInLei3dScreen() {

    }

    void MadeInLei3dScreen::HandleInput() {

    }
} // namespace lei3d