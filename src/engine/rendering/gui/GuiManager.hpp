#pragma once
#include <unordered_map>

#include "fontrenderer/FontRenderer.hpp"
#include "rendering/Shader.hpp"

namespace lei3d 
{
	class GuiComponent;

	class GuiManager
    {
    private:
		std::unordered_map<unsigned, GuiComponent*> s_guiComponents;

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

		void RenderGui(const glm::vec2& screenSize);
		void UpdateGui();
    };
}
