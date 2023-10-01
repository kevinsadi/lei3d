#include "FontRenderer.hpp"

#include "TGATexture.hpp"
#include "core/Application.hpp"
#include "logging/GLDebug.hpp"
#include "rendering/Shader.hpp"
#include "rendering/gui/UiMesh.hpp"

namespace lei3d
{
	std::vector<Font> FontRenderer::s_fonts;

	FontRenderer::FontRenderer()
	{
	}

	FontRenderer::~FontRenderer()
	{
		delete m_textShader;
		s_fonts.clear();
	}

	void FontRenderer::Init()
	{
		s_fonts.emplace_back("britannic");

		m_textShader = new Shader("./data/shaders/text.vert", "./data/shaders/text.frag");
	}

	void FontRenderer::RenderText(std::string text, float x, float y, float scale, glm::vec4 color, glm::vec2 screenSize)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		m_textShader->bind();

		std::vector<UiMesh::Vertex> vertices;
		std::vector<unsigned int> indices;
		
		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f)) * 
			glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, 1.0f));

		m_textShader->setUniformMat4("transform", transform);
		m_textShader->setVec2("screenSize", screenSize);
		m_textShader->setInt("ourTexture", 0);

		float xOffset = 0;
		// Iterate through all characters
		for (char c : text)
		{
			Character ch = s_fonts[m_fontIndex].m_characters[c];

			float xpos = xOffset + ch.XOffset;
			float ypos = 0;

			float w = ch.Size.x;
			float h = ch.Size.y;

			vertices.push_back(UiMesh::Vertex({ xpos, ypos + h }, { color.x, color.y, color.z, color.w }, { ch.TopLeft.x, ch.TopLeft.y }));
			vertices.push_back(UiMesh::Vertex({ xpos + w, ypos }, { color.x, color.y, color.z, color.w }, { ch.BottomRight.x, ch.BottomRight.y }));
			vertices.push_back(UiMesh::Vertex({ xpos + w, ypos + h }, { color.x, color.y, color.z, color.w }, { ch.BottomRight.x, ch.TopLeft.y }));
			vertices.push_back(UiMesh::Vertex({ xpos, ypos }, { color.x, color.y, color.z, color.w }, { ch.TopLeft.x, ch.BottomRight.y }));

			indices.push_back(vertices.size() - 4);
			indices.push_back(vertices.size() - 2);
			indices.push_back(vertices.size() - 1);

			indices.push_back(vertices.size() - 2);
			indices.push_back(vertices.size() - 3);
			indices.push_back(vertices.size() - 1);

			xOffset += ch.XAdvance;
		}

		UiMesh mesh = UiMesh(vertices, indices, *s_fonts[m_fontIndex].m_texture);
		mesh.Draw();

		glDisable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ZERO);
	}
}
