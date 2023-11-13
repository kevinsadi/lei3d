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
		const std::pair<Space, glm::vec2>& pos ,
		const std::pair<LineHeightMetric, float>& fontSize,
		const glm::vec4& textColor,
		const glm::vec4& backgroundColor,
		std::function<void()> onClick,
		std::function<void()> onHover,
		std::function<void()> onStopHover
	)
		: GuiRect(anchor, pos, { Space::NORMALIZED, { 0, 0 } }, backgroundColor, -1, onClick, onHover, onStopHover)
		, m_text(text)
		, m_fontSize(fontSize)
		, m_textColor(textColor)
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

	void GuiTextBox::Render(const glm::vec2& screenSize)
	{
		UpdateBackgroundSize(screenSize);

		if (abs(m_color.w - 0) > 0.0001f)
		{
			GuiRect::Render(screenSize);
		}

		m_pShader->bind();

		m_pShader->setUniformMat4("translation", translate(glm::identity<glm::mat4>(),PosNormalized(screenSize)));

		m_pShader->setUniformMat4("scale", scale(glm::identity<glm::mat4>(), glm::vec3(GetFontScalarNormalized(screenSize), 1)));
		
		m_pShader->setVec4("color", m_textColor);

		m_pShader->setInt("useTex", 1);
		m_pShader->setInt("ourTexture", 0);

		m_textMesh->Draw(m_pShader);
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
