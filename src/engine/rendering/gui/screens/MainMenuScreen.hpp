#pragma once
#include "GuiScreen.hpp"

namespace lei3d
{
	class MainMenuScreen : public GuiScreen
	{
    private:
        int GetSplashTexId();

	protected:
		void Init() override;

    public:
        MainMenuScreen();
        ~MainMenuScreen() override;
	};
} 