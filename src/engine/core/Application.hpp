#pragma once

#include "core/SceneManager.hpp"
#include "guitools/AppGUI.hpp"
#include "rendering/PrimitiveRenderer.hpp"
#include "rendering/RenderSystem.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <filesystem>
#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include <chrono>
#include <thread>

namespace lei3d
{
	class AppGUI;
	class SceneManager;

	class Application
	{
	private:
		static Application* s_Instance;

		const unsigned int SCREEN_WIDTH = 1200;
		const unsigned int SCREEN_HEIGHT = 1000;

		GLFWwindow*		   m_Window = nullptr;

		std::unique_ptr<AppGUI>		  m_AppGUI;
		std::unique_ptr<SceneManager> m_SceneManager;

		RenderSystem m_Renderer;
		PrimitiveRenderer m_PrimitiveRenderer;

		// NOTE: Don't modify this directly. Use SetUIActive.
		bool m_UIActive = false;

		float m_LastFrameTime = 0.0f; // used to keep track of delta time
		float m_DeltaTime = 0.0f;	  // Total time for last frame.
		float m_DesiredFPS =
			120.0f; // FPS will be capped to this value. (current bug means that the FPS cap is half, not sure why)
	public:
		Application();
		~Application();

		void Run(); // Run the app.

		void SetUIActive(bool uiActive);

		static GLFWwindow* Window();
		static float	   DeltaTime();
		static PrimitiveRenderer& PrimitiveRenderer();

	private:
		void Initialize(); // Start the App
		void FrameTick();  // Called every frame

		void Update();
		void Render();
		void ImGuiRender();

		void SetupInputCallbacks();
		void ProcessKeyboardInput(GLFWwindow* window, int key, int scancode, int action, int mods);
	};
} // namespace lei3d