#pragma once
#include "GuiScreen.hpp"

namespace lei3d
{
	class MainMenuScreen : public GuiScreen
	{
	private:
		int m_closeButtonId = -1;

	protected:
		void Init() override;

    public:
        MainMenuScreen();
        ~MainMenuScreen() override;

		void Update(const glm::vec2& screenSize, const glm::vec2& mousePos) override;
	};
} 