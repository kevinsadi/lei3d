#pragma once

#include <imgui.h>

namespace lei3d
{
	class EditorGUI
	{
	private:
		bool m_ShowDemoWindow = false;

	public:
		void RenderUI(); // DON"T MAKE THIS CONST
		static void renderDebugCursor();
	};
} // namespace lei3d
