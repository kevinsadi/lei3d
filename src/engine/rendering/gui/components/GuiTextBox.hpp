#pragma once
#include <string>

#include "GuiComponent.hpp"
#include "GuiRect.hpp"

namespace lei3d
{
    class GuiTextBox : public GuiRect
	{
    public:
		enum class LineHeightMetric
		{
			PT,
			PX,
			NORM,
		};

		GuiTextBox(
			const std::string& text = "TextBox",
			Anchor anchor = Anchor::CENTER,
			const std::pair<Space, glm::vec2>& pos = { Space::NORMALIZED, { 0.25, 0.25 } },
			const std::pair<LineHeightMetric, float>& fontSize = { LineHeightMetric::PT, 100 },
			const glm::vec4& textColor = { 1, 1, 1, 1 },
			const glm::vec4& backgroundColor = { 1, 0, 0, 0.5 },
			std::function<void()> onClick = nullptr,
			std::function<void()> onHover = nullptr,
			std::function<void()> onStopHover = nullptr
		);
		~GuiTextBox() override;

		void SetText(const std::string& text);
		void SetFontSizePt(float fontSize);
		void SetFontSizePx(float fontSize);
		void SetFontSizeNorm(float fontSize);
		void SetTextColor(const glm::vec4& textColor);
		void SetBackgroundColor(const glm::vec4& backgroundColor);

		void Render(const glm::vec2& screenSize) override;
		void Update() override;
		
	protected:
		std::string m_text;
		std::pair<LineHeightMetric, float> m_fontSize;
		glm::vec4 m_textColor;
		UiMesh* m_textMesh = nullptr;

    private:
		glm::vec2 GetFontScalarNormalized(const glm::vec2& screenSize) const;

		void GenerateMesh();
		void UpdateBackgroundSize(const glm::vec2& screenSize);
	};
} 
