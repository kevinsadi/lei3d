#pragma once

#include <glm/vec4.hpp>

#include "Font.hpp"

namespace lei3d
{
	class UiMesh;
	class Shader;

    class FontRenderer
    {
	private:
		friend class GuiManager;
		friend class GuiTextBox;

		std::vector<Font> m_fonts;

		FontRenderer();
		~FontRenderer();

		void Init();

		UiMesh* CreateMesh(const std::string& text, unsigned fontIndex = 0);
		float GetTextWidthPx(const std::string& text, float lineHeightPx, unsigned fontIndex = 0);

		static float PtToPx(float pt);
	};
}
