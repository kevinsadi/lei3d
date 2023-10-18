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

	void FontRenderer::RenderText(std::string text, float x, float y, float scale, glm::vec4 color, glm::vec2 screenSize, Shader* shader)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		shader->bind();

		std::vector<UiMesh::Vertex> vertices;
		std::vector<unsigned int> indices;
		
		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f)) * 
			glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, 1.0f));

		shader->setUniformMat4("transform", transform);
		shader->setVec2("screenSize", screenSize);
		shader->setInt("ourTexture", 0);
		shader->setVec4("color", color);
		shader->setInt("normalized", false);

		float xOffset = 0;
		// Iterate through all characters
		for (char c : text)
		{
			Character ch = s_fonts[m_fontIndex].m_characters[c];

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

		UiMesh mesh = UiMesh(vertices, indices, s_fonts[m_fontIndex].m_texture->m_GlID);
		mesh.Draw(GuiManager::Instance().m_guiFontShader);

		glDisable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ZERO);
	}
}
