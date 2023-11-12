#pragma once

#include <string>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace lei3d
{
	class TGATexture;

	struct Character
	{
		glm::vec2 TopLeft;
		glm::vec2 BottomRight;
		glm::vec2 Size;
		float	  XOffset;
		float	  YOffset;	
		float	  XAdvance;
	};

	class Font
	{
	private:
		friend class FontRenderer;
		float m_fontSize;
		float m_lineHeightPx;

		void Load(const std::string& filename);
	public:
		std::string m_fontName;
		TGATexture* m_texture;
		Character m_characters[256];

		Font(const std::string& fontname);
		~Font();
	};
}
