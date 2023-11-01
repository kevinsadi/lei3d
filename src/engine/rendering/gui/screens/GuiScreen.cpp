#include "GuiScreen.hpp"

#include "core/InputManager.hpp"
#include "rendering/gui/components/GuiComponent.hpp"
#include "rendering/gui/components/GuiTextBox.hpp"

namespace lei3d
{
	GuiScreen::GuiScreen()
	{
		
	}

	GuiScreen::~GuiScreen()
	{
		for (auto& component : m_components)
		{
			delete component.second;
		}
	}

	void GuiScreen::Init()
	{
		GuiTextBox* text = new GuiTextBox("Hello");

		AddComponent(text);
	}

	void GuiScreen::AddComponent(GuiComponent* guiComponent)
	{
		m_components[guiComponent->GetId()] = guiComponent;
	}

	bool GuiScreen::RemoveComponent(unsigned id)
	{
		auto it = m_components.find(id);

		if (it != m_components.end())
		{
			m_components.erase(it);
			return true;
		}

		return false;
	}

	void GuiScreen::Update(const glm::vec2& screenSize, const glm::vec2& mousePos)
	{
		for (auto& guiComponent : m_components)
		{
			guiComponent.second->Update();
		}

		if (InputManager::GetInstance().isButtonDown(GLFW_MOUSE_BUTTON_LEFT))
		{
			for (auto& guiComponent : m_components)
			{
				if (guiComponent.second->IsMouseOver(screenSize, mousePos))
				{
					guiComponent.second->OnClick(screenSize, mousePos);
				}
			}
		}
	}

	void GuiScreen::Render(const glm::vec2& screenSize)
	{
		for (auto& component : m_components)
		{
			component.second->Render(screenSize);
		}
	}
}
