#pragma once
#include <unordered_map>
#include <unordered_set>

#include "fontrenderer/FontRenderer.hpp"
#include "rendering/Shader.hpp"

namespace lei3d 
{
	class GuiComponent;

	class GuiManager
    {
    private:
		std::unordered_map<unsigned, GuiComponent*> m_guiComponents;
		std::unordered_set<unsigned> m_interactableComponents;

		std::unordered_set<unsigned> m_mouseOver;

		GuiManager();

	public:
		Shader m_guiShader;
		Shader m_guiTextureShader;
		Shader m_guiFontShader;

		FontRenderer m_fontRenderer;

		static GuiManager& Instance();

		void Init();

		void AddGuiComponent(GuiComponent* guiComponent);
		bool RemoveGuiComponent(unsigned id);

		void SetInteractable(unsigned id, bool interactable);
		bool InteractableLayer();

		void RenderGui(const glm::vec2& screenSize);
		void UpdateGui();
		void SendClick(const glm::vec2& screenSize, const glm::vec2& mousePos);
    };
}
