#pragma once

#include <glm/vec4.hpp>

#include "Font.hpp"

namespace lei3d
{
	class Shader;

    class FontRenderer
    {
	public:
		static std::vector<Font> s_fonts;
		unsigned m_fontIndex = 0;

		FontRenderer();
		~FontRenderer();

		void Init();
		void RenderText(std::string text, float x, float y, float scale, glm::vec4 color, glm::vec2 screenSize);

	private:
		Shader* m_textShader;
	};
}
