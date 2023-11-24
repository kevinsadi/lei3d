#include "GuiManager.hpp"

#include "components/GuiComponent.hpp"
#include "core/Application.hpp"
#include "core/InputManager.hpp"
#include "screens/BaseGuiScreen.hpp"
#include "screens/GuiScreen.hpp"
#include "logging/Log.hpp"

namespace lei3d 
{

	void GuiManager::SetActiveScreen(GuiScreen* screen)
	{
		if (m_activeScreen)
		{
			delete m_activeScreen;
			m_activeScreen = nullptr;
		}

		if (screen == nullptr)
		{
			SceneManager::GetInstance().ActiveScene().Play();
		}
		else
		{
			if (!screen->m_initialized)
			{
				screen->Init();
			}

			SceneManager::GetInstance().ActiveScene().Pause();
			InputManager::GetInstance().giveInputFocus(InputManager::InputTarget::GUI);
		}

		m_activeScreen = screen;
	}

	GuiManager::GuiManager()
	{
		
	}

	GuiManager& GuiManager::Instance()
	{
		static GuiManager s_instance;
		return s_instance;
	}

	void GuiManager::Init()
	{
		m_guiShader = Shader("./data/shaders/gui.vert", "./data/shaders/gui.frag");
		
		m_fontRenderer.Init();

		m_baseScreen = new BaseGuiScreen();
		m_baseScreen->Init();
	}

	GuiScreen& GuiManager::GetBaseScreen()
	{
		return *m_baseScreen;
	}

	GuiScreen& GuiManager::GetActiveScreen()
	{
		return *m_activeScreen;
	}

	void GuiManager::CloseActiveScreen()
	{
		QueueNextScreen(nullptr);
	}

	void GuiManager::QueueNextScreen(GuiScreen* screen)
	{
		m_nextScreen = screen;
		m_shouldSwapScreens = true;
	}

	void GuiManager::RenderGui(const glm::vec2& screenSize)
	{
		if (!(m_activeScreen && m_activeScreen->m_shouldHideHUD))
			m_baseScreen->Render(screenSize);

		if (m_activeScreen)
			m_activeScreen->Render(screenSize);
	}

	void GuiManager::UpdateGui(const glm::vec2& screenSize, const glm::vec2& mousePos)
	{
		m_baseScreen->Update(screenSize, mousePos);

		if (m_shouldSwapScreens)
		{
			SetActiveScreen(m_nextScreen);
			m_nextScreen = nullptr;
			m_shouldSwapScreens = false;
		}

		if (m_activeScreen)
		{
			m_activeScreen->HandleInput();
			m_activeScreen->Update(screenSize, mousePos);
		}
	}
}
