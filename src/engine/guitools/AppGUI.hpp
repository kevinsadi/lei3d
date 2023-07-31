#pragma once

#include <imgui.h>

#include "engine/Application.hpp"

namespace lei3d {
	class Application;

	class AppGUI {
	private:
		bool m_ShowDemoWindow = true;
	public:
		void RenderUI();
	};
}