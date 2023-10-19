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
		const std::pair<LineHeightMetrix, float>& fontSize, 
		glm::vec4 textColor, 
		glm::vec4 backgroundColor, 
		bool backgroundEnabled
	)
		: GuiRect(anchor, pos)
		, m_text(text)
		, m_fontSize(fontSize)
		, m_textColor(textColor)
		, m_backgroundEnabled(backgroundEnabled)
	{
		GenerateMesh();
		m_color = backgroundColor;
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
		m_fontSize = { LineHeightMetrix::PT, fontSize };
	}

	void GuiTextBox::SetFontSizePx(float fontSize)
	{
		m_fontSize = { LineHeightMetrix::PX, fontSize };
	}

	void GuiTextBox::SetFontSizeNorm(float fontSize)
	{
		m_fontSize = { LineHeightMetrix::NORM, fontSize };
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

		GuiManager::Instance().m_guiFontShader->bind();

		GuiManager::Instance().m_guiFontShader->setUniformMat4("transform",
			glm::translate(glm::identity<glm::mat4>(), PosNormalized(screenSize)) *
			glm::scale(glm::identity<glm::mat4>(), glm::vec3(GetFontScalar(screenSize), 1))
		);

		GuiManager::Instance().m_guiFontShader->setVec2("screenSize", screenSize);
		GuiManager::Instance().m_guiFontShader->setVec4("color", m_textColor);
		GuiManager::Instance().m_guiFontShader->setInt("normalized", true);
		GuiManager::Instance().m_guiFontShader->setInt("ourTexture", 0);

		m_textMesh->Draw(GuiManager::Instance().m_guiFontShader);
	}

	void GuiTextBox::Update()
	{
		GuiRect::Update();
	}

	glm::vec2 GuiTextBox::GetFontScalar(const glm::vec2& screenSize) const
	{
		switch (m_fontSize.first)
		{
		case LineHeightMetrix::PT:
			return { m_fontSize.second / screenSize.x, m_fontSize.second / screenSize.y };
		case LineHeightMetrix::PX:
			return { m_fontSize.second * (4.f / 3.f) / screenSize.x, m_fontSize.second * (4.f / 3.f) / screenSize.y };
		case LineHeightMetrix::NORM:
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
		glm::vec2 fontScalar = GetFontScalar(screenSize);

		m_size.first = Space::NORMALIZED;
		m_size.second = { GuiManager::Instance().m_fontRenderer.GetTextWidth(m_text) * fontScalar.x, fontScalar.y };
	}
} // namespace lei3d
