#pragma once
#include "GuiScreen.hpp"

namespace lei3d
{
	class MainMenuScreen : public GuiScreen
	{
	protected:
		void Init() override;

    public:
        MainMenuScreen();
        ~MainMenuScreen() override;
	};
} 