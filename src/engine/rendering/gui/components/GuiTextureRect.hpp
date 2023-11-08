#pragma once
#include "GuiComponent.hpp"

namespace lei3d
{
	class GuiTextureRect : public GuiComponent
	{
	private:
		int m_textureID;
		float m_alpha;

	public:
		GuiTextureRect(
			int textureID,
			Anchor anchor = Anchor::CENTER,
			const std::pair<Space, glm::vec2>& pos = { Space::NORMALIZED, { 0.25, 0.25 } },
			const std::pair<Space, glm::vec2>& size = { Space::NORMALIZED, { 0.5, 0.5 } },
			float alpha = 1.0f,
			std::function<void()> onClick = nullptr
		);
		~GuiTextureRect() override;

		void Render(const glm::vec2& screenSize) override;
		void Update() override;

		bool IsMouseOver(const glm::vec2& screenSize, const glm::vec2& mousePosition) const override;
	};
} 