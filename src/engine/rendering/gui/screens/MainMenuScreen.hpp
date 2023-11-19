#pragma once
#include "GuiScreen.hpp"

namespace lei3d
{
	class MainMenuScreen : public GuiScreen
	{
	private:
		static int GetLei3dTexture();
		static int GetSkyLeiTexture();
		bool m_enteredHover = false;

	protected:
		void Init() override;

	public:
		MainMenuScreen();
		~MainMenuScreen() override;

		void HandleInput() override;
	};
} // namespace lei3d
