#pragma once
#include "GuiScreen.hpp"

namespace lei3d
{
	class PauseMenuScreen : public GuiScreen
	{
	protected:
		void Init() override;

    public:
        PauseMenuScreen();
        ~PauseMenuScreen() override;
	};
} 