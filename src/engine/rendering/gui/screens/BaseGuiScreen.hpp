#pragma once
#include "GuiScreen.hpp"

namespace lei3d
{
	class BaseGuiScreen : public GuiScreen
	{
	protected:
		void Init() override;

	public:
		BaseGuiScreen();
		~BaseGuiScreen() override;
	};
}
