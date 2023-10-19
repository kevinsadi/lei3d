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
		// px = 3/4 * pt
		float fontSize;

		void Load(const char* filename);
	public:
		const char* m_fontName;
		TGATexture* m_texture;
		Character m_characters[256];

		Font(const char* fontname);
		~Font();
	};
}
