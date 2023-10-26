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
		m_guiComponents[guiComponent->m_id] = guiComponent;

		if (guiComponent->m_interactable)
		{
			m_interactableComponents.insert(guiComponent->m_id);
		}
	}

	bool GuiManager::RemoveGuiComponent(unsigned id)
	{
		auto it = m_guiComponents.find(id);

		if (it != m_guiComponents.end())
		{
			if (it->second->m_interactable)
			{
				m_interactableComponents.erase(id);
			}

			m_guiComponents.erase(it);
			return true;
		}

		return false;
	}

	void GuiManager::SetInteractable(unsigned id, bool interactable)
	{
		if (interactable)
		{
			m_interactableComponents.insert(id);
		}
		else
		{
			m_interactableComponents.erase(id);
		}
	}

	bool GuiManager::InteractableLayer()
	{
		return !m_interactableComponents.empty();
	}

	void GuiManager::RenderGui(const glm::vec2& screenSize)
	{
		for (auto& guiComponent : m_guiComponents)
		{
			guiComponent.second->BeginRender();
			guiComponent.second->Render(screenSize);
			guiComponent.second->EndRender();
		}
	}

	void GuiManager::UpdateGui()
	{
		m_mouseOver.clear();

		for (auto& guiComponent : m_guiComponents)
		{
			guiComponent.second->Update();
		}
	}

	void GuiManager::SendClick(const glm::vec2& screenSize, const glm::vec2& mousePos)
	{
		for (auto& guiComponent : m_guiComponents)
		{
			if (guiComponent.second->IsInteractable() && guiComponent.second->IsMouseOver(screenSize, mousePos))
			{
				guiComponent.second->OnClick(screenSize, mousePos);
			}
		}
	}
}
