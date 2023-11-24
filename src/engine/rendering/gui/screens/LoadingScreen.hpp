#pragma once
#include "GuiScreen.hpp"

namespace lei3d
{
	class GuiTextBox;

	class LoadingScreen : public GuiScreen
	{
	public:
		LoadingScreen();
		~LoadingScreen() override;

		void HandleInput() override;
		void Update(const glm::vec2& screenSize, const glm::vec2& mousePos) override;

	protected:
		void Init() override;
	};
}
