#pragma once
#include "GuiScreen.hpp"

namespace lei3d
{
	class ControlsScreen : public GuiScreen
	{
	private:
		int GetControlsTexID();

	protected:
		void Init() override;

	public:
		ControlsScreen();
		~ControlsScreen() override;
	};
}
