#include "GuiManager.hpp"

#include "components/GuiComponent.hpp"
#include "core/Application.hpp"

namespace lei3d 
{
	GuiManager* GuiManager::s_instance = new GuiManager();

	GuiManager& GuiManager::Instance()
	{
		return *s_instance;
	}

	void GuiManager::Init()
	{
		s_instance->m_guiFontShader = new Shader("./data/shaders/gui.vert", "./data/shaders/guifont.frag");
		s_instance->m_guiShader = new Shader("./data/shaders/gui.vert", "./data/shaders/gui.frag");
		s_instance->m_guiTextureShader = new Shader("./data/shaders/gui.vert", "./data/shaders/guitexture.frag");
		
		s_instance->m_fontRenderer.Init();
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
