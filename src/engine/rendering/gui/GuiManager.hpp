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
		static GuiManager* s_instance;
		std::unordered_map<unsigned, GuiComponent*> s_guiComponents;

		GuiManager() = default;
		~GuiManager() = default;

	public:
		Shader* m_guiShader = nullptr;
		Shader* m_guiTextureShader = nullptr;
		Shader* m_guiFontShader = nullptr;

		FontRenderer m_fontRenderer = FontRenderer();

		static GuiManager& Instance();

		static void Init();

		void AddGuiComponent(GuiComponent* guiComponent);
		bool RemoveGuiComponent(unsigned id);

		void RenderGui(const glm::vec2& screenSize);
		void UpdateGui();
    };
}
