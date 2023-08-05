#pragma once

#include <imgui.h>

#include "core/Application.hpp"

namespace lei3d {
	class Application;
	class Scene;

	class AppGUI {
	private:
		bool m_ShowDemoWindow = false;
	public:
		void RenderUI();
		void AddScene(std::string name, const Scene& scene);
	};
}