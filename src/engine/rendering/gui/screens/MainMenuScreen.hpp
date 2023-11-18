#pragma once
#include "GuiScreen.hpp"

namespace lei3d
{
	class MainMenuScreen : public GuiScreen
	{
	private:
		static int GetLei3dTexture();
		static int GetSkyLeiTexture();

	protected:
		void Init() override;

	public:
		MainMenuScreen();
		~MainMenuScreen() override;

		void HandleInput() override;
	};
}
