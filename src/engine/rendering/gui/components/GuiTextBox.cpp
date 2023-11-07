#include "GuiTextBox.hpp"

#include "rendering/gui/GuiManager.hpp"
#include "rendering/gui/UiMesh.hpp"
#include "rendering/gui/fontrenderer/FontRenderer.hpp"

namespace lei3d
{
	GuiTextBox::GuiTextBox
	(
		const std::string& text, 
		Anchor anchor,
		const std::pair<Space, glm::vec2>& pos,
		std::function<void()> onClick,
		const std::pair<LineHeightMetric, float>& fontSize, 
		glm::vec4 textColor, 
		glm::vec4 backgroundColor, 
		bool backgroundEnabled
	)
		: GuiRect(anchor, pos, { Space::NORMALIZED, { 0.25, 0.25 } }, onClick, backgroundColor)
		, m_text(text)
		, m_fontSize(fontSize)
		, m_textColor(textColor)
		, m_backgroundEnabled(backgroundEnabled)
	{
		GenerateMesh();
	}

	GuiTextBox::~GuiTextBox()
	{
		delete m_textMesh;
	}

	void GuiTextBox::SetText(const std::string& text)
	{
		m_text = text;
		GenerateMesh();
	}

	void GuiTextBox::SetFontSizePt(float fontSize)
	{
		m_fontSize = { LineHeightMetric::PT, fontSize };
	}

	void GuiTextBox::SetFontSizePx(float fontSize)
	{
		m_fontSize = { LineHeightMetric::PX, fontSize };
	}

	void GuiTextBox::SetFontSizeNorm(float fontSize)
	{
		m_fontSize = { LineHeightMetric::NORM, fontSize };
	}

	void GuiTextBox::SetTextColor(const glm::vec4& textColor)
	{
		m_textColor = textColor;
	}

	void GuiTextBox::SetBackgroundColor(const glm::vec4& backgroundColor)
	{
		m_color = backgroundColor;
	}

	void GuiTextBox::SetBackgroundEnabled(bool backgroundEnabled)
	{
		m_backgroundEnabled = backgroundEnabled;
	}

	void GuiTextBox::Render(const glm::vec2& screenSize)
	{
		if (m_backgroundEnabled)
		{
			UpdateBackgroundSize(screenSize);
			GuiRect::Render(screenSize);
		}

		GuiManager::Instance().m_guiTextureShader.bind();

		GuiManager::Instance().m_guiTextureShader.setUniformMat4("translation",
			glm::translate(glm::identity<glm::mat4>(),PosNormalized(screenSize))
		);

		GuiManager::Instance().m_guiTextureShader.setUniformMat4("scale",
			glm::scale(glm::identity<glm::mat4>(), glm::vec3(GetFontScalarNormalized(screenSize), 1))
		);
		
		GuiManager::Instance().m_guiTextureShader.setVec4("color", m_textColor);
		GuiManager::Instance().m_guiTextureShader.setInt("ourTexture", 0);

		m_textMesh->Draw(&GuiManager::Instance().m_guiTextureShader);
	}

	void GuiTextBox::Update()
	{
		GuiRect::Update();
	}

	glm::vec2 GuiTextBox::GetFontScalarNormalized(const glm::vec2& screenSize) const
	{
		switch (m_fontSize.first)
		{
		case LineHeightMetric::PT:
				return { FontRenderer::PtToPx(m_fontSize.second) / screenSize.x, FontRenderer::PtToPx(m_fontSize.second) / screenSize.y };
		case LineHeightMetric::PX:
			return { m_fontSize.second / screenSize.x, m_fontSize.second / screenSize.y };
		case LineHeightMetric::NORM:
			return { m_fontSize.second * screenSize.y / screenSize.x, m_fontSize.second };
		default:
			return { 1, 1 };
		}
	}

	void GuiTextBox::GenerateMesh()
	{
		m_textMesh = GuiManager::Instance().m_fontRenderer.CreateMesh(m_text);
	}

	void GuiTextBox::UpdateBackgroundSize(const glm::vec2& screenSize)
	{
		if (m_fontSize.first == LineHeightMetric::PT)
		{
			m_size.first = Space::PIXELS;
			m_size.second = { GuiManager::Instance().m_fontRenderer.GetTextWidthPx(m_text, FontRenderer::PtToPx(m_fontSize.second)), FontRenderer::PtToPx(m_fontSize.second) };
			return;
		}
		else if (m_fontSize.first == LineHeightMetric::PX)
		{
			m_size.first = Space::PIXELS;
			m_size.second = { GuiManager::Instance().m_fontRenderer.GetTextWidthPx(m_text, m_fontSize.second), m_fontSize.second };
			return;
		}
		else if (m_fontSize.first == LineHeightMetric::NORM)
		{
			m_size.first = Space::NORMALIZED;
			m_size.second = { GuiManager::Instance().m_fontRenderer.GetTextWidthPx(m_text, m_fontSize.second * screenSize.x) / screenSize.x, m_fontSize.second };
			return;
		}
	}
} // namespace lei3d
