#pragma once
#include <functional>

#include "GuiRect.hpp"

namespace lei3d
{
    class GuiRectButton : public GuiRect
    {
    public:
		GuiRectButton(
			std::function<void()> onClick = []() {},
			Anchor anchor = Anchor::CENTER,
			const std::pair<Space, glm::vec2>& pos = { Space::NORMALIZED, { 0.25, 0.25 } },
			const std::pair<Space, glm::vec2>& size = { Space::NORMALIZED, { 0.5, 0.5 } },
			glm::vec4 color = { 1, 0, 0, 0.5 }
		);

    private:
		std::function<void()> m_onClick;

		void Update() override;
		void OnClick(const glm::vec2& screenSize, const glm::vec2& mousePosition) override;
		bool IsMouseOver(const glm::vec2& screenSize, const glm::vec2& mousePosition) const override;
    };
}
