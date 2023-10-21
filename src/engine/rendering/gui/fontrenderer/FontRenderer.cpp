#include "FontRenderer.hpp"

#include "TGATexture.hpp"
#include "core/Application.hpp"
#include "logging/GLDebug.hpp"
#include "rendering/Shader.hpp"
#include "rendering/gui/GuiManager.hpp"
#include "rendering/gui/UiMesh.hpp"

namespace lei3d
{
	std::vector<Font> FontRenderer::s_fonts;

	FontRenderer::FontRenderer()
	{
	}

	FontRenderer::~FontRenderer()
	{
		s_fonts.clear();
	}

	void FontRenderer::Init()
	{
		s_fonts.emplace_back("britannic");
	}

	UiMesh* FontRenderer::CreateMesh(std::string text, unsigned fontIndex)
	{
		std::vector<UiMesh::Vertex> vertices;
		std::vector<unsigned int> indices;

		float xOffset = 0;

		// Iterate through all characters
		for (char c : text)
		{
			Character ch = s_fonts[fontIndex].m_characters[c];

			float xpos = xOffset + ch.XOffset;
			float ypos = 0;

			float w = ch.Size.x;
			float h = ch.Size.y;

			vertices.push_back(UiMesh::Vertex({ xpos, ypos + h }, { ch.TopLeft.x, ch.TopLeft.y }));
			vertices.push_back(UiMesh::Vertex({ xpos + w, ypos }, { ch.BottomRight.x, ch.BottomRight.y }));
			vertices.push_back(UiMesh::Vertex({ xpos + w, ypos + h }, { ch.BottomRight.x, ch.TopLeft.y }));
			vertices.push_back(UiMesh::Vertex({ xpos, ypos }, { ch.TopLeft.x, ch.BottomRight.y }));

			indices.push_back(vertices.size() - 4);
			indices.push_back(vertices.size() - 2);
			indices.push_back(vertices.size() - 1);

			indices.push_back(vertices.size() - 2);
			indices.push_back(vertices.size() - 3);
			indices.push_back(vertices.size() - 1);

			xOffset += ch.XAdvance;
		}

		return new UiMesh(vertices, indices, s_fonts[fontIndex].m_texture->m_GlID);
	}

	float FontRenderer::GetTextWidth(std::string text, unsigned fontIndex)
	{
		float width = 0;

		// Iterate through all characters
		for (char c : text)
		{
			Character ch = s_fonts[fontIndex].m_characters[c];
			width += ch.XAdvance;
		}

		return width;
	}

	float FontRenderer::PtToPx(float pt)
	{
		return pt * 4.f / 3.f;
	}
}
