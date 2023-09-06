#pragma once

#include <imgui.h>

// This takes the place of the in-game gui for when we separate editor from game.
namespace lei3d
{
	class GameGUI
	{
	public:
		void RenderUI(); // DON"T MAKE THIS CONST
	};
} // namespace lei3d