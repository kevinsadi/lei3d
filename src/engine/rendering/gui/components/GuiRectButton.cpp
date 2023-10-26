#include "GuiRectButton.hpp"

#include <functional>

namespace lei3d
{
	GuiRectButton::GuiRectButton(
		std::function<void()> onClick,
		Anchor anchor,
		const std::pair<Space, glm::vec2>& pos,
		const std::pair<Space, glm::vec2>& size,
		glm::vec4 color
	)
		: GuiRect(anchor, pos, size, color)
		, m_onClick(onClick)
	{
		SetInteractable(true);
	}

	void GuiRectButton::Update()
	{
		GuiRect::Update();
	}

	void GuiRectButton::OnClick(const glm::vec2& screenSize, const glm::vec2& mousePosition)
	{
		if (m_onClick)
		{
			m_onClick();
		}
	}

	bool GuiRectButton::IsMouseOver(const glm::vec2& screenSize, const glm::vec2& mousePosition) const
	{
		glm::vec3 pos = PosPixels(screenSize);
		glm::vec2 size = SizePixels(screenSize);

		return mousePosition.x >= pos.x && mousePosition.x <= pos.x + size.x
			&& mousePosition.y >= pos.y && mousePosition.y <= pos.y + size.y;
	}
}
