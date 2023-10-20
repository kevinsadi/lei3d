#include "GuiManager.hpp"

#include "components/GuiComponent.hpp"
#include "core/Application.hpp"

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
	}

	void GuiManager::AddGuiComponent(GuiComponent* guiComponent)
	{
		s_guiComponents[guiComponent->m_id] = guiComponent;
	}

	bool GuiManager::RemoveGuiComponent(unsigned id)
	{
		auto it = s_guiComponents.find(id);

		if (it != s_guiComponents.end())
		{
			s_guiComponents.erase(it);
			return true;
		}

		return false;
	}

	void GuiManager::RenderGui(const glm::vec2& screenSize)
	{
		for (auto& guiComponent : s_guiComponents)
		{
			guiComponent.second->BeginRender();
			guiComponent.second->Render(screenSize);
			guiComponent.second->EndRender();
		}
	}

	void GuiManager::UpdateGui()
	{
		for (auto& guiComponent : s_guiComponents)
		{
			guiComponent.second->Update();
		}
	}
}
