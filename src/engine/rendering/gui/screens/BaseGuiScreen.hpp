#pragma once
#include "GuiScreen.hpp"

namespace lei3d
{
	class BaseGuiScreen : public GuiScreen
	{
	private:
		int m_colorBarId = -1;

	protected:
		void Init() override;

	public:
		BaseGuiScreen();
		~BaseGuiScreen() override;

		void Update(const glm::vec2& screenSize, const glm::vec2& mousePos) override;
	};
}
