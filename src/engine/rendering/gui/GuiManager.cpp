#include "GuiManager.hpp"

#include "components/GuiComponent.hpp"
#include "core/Application.hpp"
#include "core/InputManager.hpp"
#include "screens/BaseGuiScreen.hpp"
#include "screens/GuiScreen.hpp"
#include "logging/Log.hpp"

namespace lei3d 
{
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
		m_guiFontShader = Shader("./data/shaders/gui.vert", "./data/shaders/guifont.frag");
		m_guiShader = Shader("./data/shaders/gui.vert", "./data/shaders/gui.frag");
		m_guiTextureShader = Shader("./data/shaders/gui.vert", "./data/shaders/guitexture.frag");
		
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

	void GuiManager::CloseActiveScreen()
	{
		SetActiveScreen(nullptr);
	}

	void GuiManager::RenderGui(const glm::vec2& screenSize)
	{
		m_baseScreen->Render(screenSize);

		if (m_activeScreen)
			m_activeScreen->Render(screenSize);
	}

	void GuiManager::UpdateGui(const glm::vec2& screenSize, const glm::vec2& mousePos)
	{
		m_baseScreen->Update(screenSize, mousePos);

		if (m_activeScreen)
		{
			if (InputManager::GetInstance().isKeyPressed(GLFW_KEY_ESCAPE, InputManager::InputTarget::GUI))
			{
				CloseActiveScreen();
			}
			else
			{
				m_activeScreen->Update(screenSize, mousePos);
			}
		}
	}
}
