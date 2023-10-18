#pragma once

#include "core/SceneManager.hpp"
#include "core/SceneView.hpp"

#include "guitools/EditorGUI.hpp"

#include "rendering/PrimitiveRenderer.hpp"
#include "rendering/RenderSystem.hpp"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "audio/AudioPlayer.hpp"
#include <filesystem>
#include <iostream>
#include <map>
#include <memory>
#include <vector>

#include <chrono>
#include <thread>

#include "rendering/gui/fontrenderer/FontRenderer.hpp"

namespace lei3d
{
	class AppGUI;
	class SceneManager;

	class Application
	{
	private:
		static Application* s_Instance;

		int screenWidth = 1200;
		int screenHeight = 1000;

		GLFWmonitor* m_Monitor = nullptr;
		GLFWwindow* m_Window = nullptr;

		// TODO: Refactor things into editor/game
		std::unique_ptr<EditorGUI> m_EditorGUI;
		std::unique_ptr<SceneManager> m_SceneManager;
		std::unique_ptr<AudioPlayer> m_AudioPlayer;
		std::unique_ptr<SceneView> m_SceneView;

		// Should we keep these on the stack? idk
		RenderSystem m_Renderer;
		PrimitiveRenderer m_PrimitiveRenderer;

		// NOTE: Don't modify this directly. Use SetUIActive.
		bool m_UIActive = false;

		float m_LastFrameTime = 0.0f; // used to keep track of delta time
		float m_DeltaTime = 0.0f;	  // Total time for last frame.
		float m_DesiredFPS = 120.0f;  // FPS will be capped to this value. (current bug
									  // means that the FPS cap is half, not sure why)

		float m_PhysicsElapsedTime = 0.0f;
		float m_FixedDeltaTime = 0.01f;

	public:
		Application();
		~Application();

		void Run(); // Run the app.

		void SetUIActive(bool uiActive);

		static GLFWwindow* Window();
		static float DeltaTime();
		static SceneView& GetSceneView();
		static PrimitiveRenderer& GetPrimitiveRenderer();

		static inline Camera& GetSceneCamera()
		{
			return GetSceneView().ActiveCamera(SceneManager::ActiveScene());
		}

	private:
		void Initialize(); // Start the App
		void FrameTick();  // Called every frame
		void GetMonitorConfiguration();

		void Update();
		void FixedUpdate();
		void Render();
		void ImGuiRender();

		void SetupInputCallbacks();
		void ProcessKeyboardInput(GLFWwindow* window, int key, int scancode,
			int action, int mods);
	};
} // namespace lei3d