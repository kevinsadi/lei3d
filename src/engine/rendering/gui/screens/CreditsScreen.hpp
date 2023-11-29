#pragma once
#include "GuiScreen.hpp"

namespace lei3d
{
	class CreditsScreen : public GuiScreen
	{
	private:
		int GetCreditsTexID();

	protected:
		void Init() override;

	public:
		CreditsScreen();
		~CreditsScreen() override;
	};
}
