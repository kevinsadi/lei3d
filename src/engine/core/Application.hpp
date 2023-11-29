#pragma once

#include "core/SceneManager.hpp"
#include "core/SceneView.hpp"

#ifdef EDITOR
	#include "guitools/EditorGUI.hpp"
#endif

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

namespace lei3d
{
	class AppGUI;

	class Application
	{
	private:
		int screenWidth = 1200;
		int screenHeight = 1000;

		GLFWmonitor* m_Monitor = nullptr;
		GLFWwindow* m_Window = nullptr;

		// TODO: Refactor things into editor/game
#ifdef EDITOR
		std::unique_ptr<EditorGUI> m_EditorGUI;
#endif
		std::unique_ptr<AudioPlayer> m_AudioPlayer;
		std::unique_ptr<SceneView> m_SceneView;

		// Should we keep these on the stack? idk
		RenderSystem m_Renderer;
		PrimitiveRenderer m_PrimitiveRenderer;

		// NOTE: Don't modify this directly. Use SetIMGUIActive.
		bool m_ImGUIActive = false;

		float m_LastFrameTime = 0.0f; // used to keep track of delta time
		float m_DeltaTime = 0.0f;	  // Total time for last frame.
		float m_DesiredFPS = 120.0f;  // FPS will be capped to this value. (current bug
									  // means that the FPS cap is half, not sure why)

		float m_PhysicsElapsedTime = 0.0f;
		float m_FixedDeltaTime = 0.015f;

	public:
		static Application& GetInstance();
		~Application();

		void Run(); // Run the app.

#ifdef EDITOR
		void SetIMGUIActive(bool uiActive);
#endif

		static GLFWwindow* Window();
		static float DeltaTime();

		SceneView& GetSceneView();
		PrimitiveRenderer& GetPrimitiveRenderer();

		inline Camera& GetSceneCamera()
		{
			return GetSceneView().ActiveCamera(SceneManager::GetInstance().ActiveScene());
		}

	private:
		Application();

		void Initialize(); // Start the App
		void FrameTick();  // Called every frame
		void GetMonitorConfiguration();

		void Update();
		void FixedUpdate();
		void Render();

#ifdef EDITOR
		void ImGuiRender();
#endif

		void ProcessInput();
	};
} // namespace lei3d