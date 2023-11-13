#pragma once
#include "GuiScreen.hpp"

namespace lei3d
{
	class SplashScreen : public GuiScreen
	{
	private:
		int GetSplashTexId();

	protected:
		void Init() override;

	public:
		SplashScreen();
		~SplashScreen() override;
	};
}
