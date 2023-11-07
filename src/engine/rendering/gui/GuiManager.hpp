#pragma once
#include <unordered_map>
#include <unordered_set>

#include "rendering/Shader.hpp"
#include "fontrenderer/FontRenderer.hpp"

namespace lei3d 
{
	class GuiScreen;
	class GuiComponent;

	class GuiManager
    {
    private:
		GuiScreen* m_baseScreen = nullptr;
		GuiScreen* m_activeScreen = nullptr;

		GuiScreen* m_nextScreen = nullptr;
		bool m_shouldSwapScreens = false;

		void SetActiveScreen(GuiScreen* screen);

		GuiManager();

	public:
		Shader m_guiShader;
		Shader m_guiTextureShader;

		FontRenderer m_fontRenderer;

		static GuiManager& Instance();

		void Init();

		GuiScreen& GetBaseScreen();
		GuiScreen& GetActiveScreen();

		void CloseActiveScreen();
		void QueueNextScreen(GuiScreen* screen);

		void RenderGui(const glm::vec2& screenSize);
		void UpdateGui(const glm::vec2& screenSize, const glm::vec2& mousePos);
    };
}
