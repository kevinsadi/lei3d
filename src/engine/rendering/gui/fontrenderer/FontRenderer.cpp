#include "FontRenderer.hpp"

#include "TGATexture.hpp"
#include "core/Application.hpp"
#include "logging/GLDebug.hpp"
#include "rendering/Shader.hpp"
#include "rendering/gui/GuiManager.hpp"
#include "rendering/gui/UiMesh.hpp"

namespace lei3d
{
	FontRenderer::FontRenderer()
	{
	}

	FontRenderer::~FontRenderer()
	{
	}

	void FontRenderer::Init()
	{
		m_fonts.emplace_back("indie_flower");
	}

	UiMesh* FontRenderer::CreateMesh(const std::string& text, unsigned fontIndex)
	{
		std::vector<UiMesh::Vertex> vertices;
		std::vector<unsigned int> indices;

		float xOffset = 0;

		// Iterate through all characters
		for (char c : text)
		{
			Character ch = m_fonts[fontIndex].m_characters[c];

			float xpos = xOffset + ch.XOffset;
			float ypos = ch.YOffset;

			float w = ch.Size.x;
			float h = ch.Size.y;

			vertices.push_back(UiMesh::Vertex({ xpos, ypos }, { ch.TopLeft.x, ch.TopLeft.y }));
			vertices.push_back(UiMesh::Vertex({ xpos + w, ypos }, { ch.BottomRight.x, ch.TopLeft.y }));
			vertices.push_back(UiMesh::Vertex({ xpos + w, ypos + h }, { ch.BottomRight.x, ch.BottomRight.y }));
			vertices.push_back(UiMesh::Vertex({ xpos, ypos + h }, { ch.TopLeft.x, ch.BottomRight.y }));

			indices.push_back(vertices.size() - 4);
			indices.push_back(vertices.size() - 3);
			indices.push_back(vertices.size() - 2);

			indices.push_back(vertices.size() - 4);
			indices.push_back(vertices.size() - 2);
			indices.push_back(vertices.size() - 1);

			xOffset += ch.XAdvance;
		}

		return new UiMesh(vertices, indices, m_fonts[fontIndex].m_texture->m_GlID);
	}

	float FontRenderer::GetTextWidthPx(const std::string& text, float lineHeightPx, unsigned fontIndex)
	{
		float width = 0;

		// Iterate through all characters
		for (char c : text)
		{
			Character ch = m_fonts[fontIndex].m_characters[c];
			width += ch.XAdvance;
		}

		return width * lineHeightPx;
	}

	float FontRenderer::PtToPx(float pt)
	{
		return pt * 4.f / 3.f;
	}
}
