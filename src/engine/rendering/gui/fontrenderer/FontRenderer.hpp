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

		static std::vector<Font> s_fonts;

		FontRenderer();
		~FontRenderer();

		void Init();

		UiMesh* CreateMesh(std::string text, unsigned fontIndex = 0);
		float GetTextWidth(std::string text, unsigned fontIndex = 0);

		float PtToPx(float pt);
	};
}
