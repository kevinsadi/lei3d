#include "EditorGUI.hpp"

#include "core/Application.hpp"
#include "core/SceneManager.hpp"

#include <string>

namespace lei3d
{
	void EditorGUI::RenderUI()
	{
		if (m_ShowDemoWindow)
		{
			ImGui::ShowDemoWindow();
		}

		bool*			 p_open;
		ImGuiWindowFlags window_flags = 0;

		// Comment/Uncomment these as needed
		p_open = NULL;
		// window_flags |= ImGuiWindowFlags_NoTitleBar;
		// window_flags |= ImGuiWindowFlags_NoScrollbar;
		window_flags |= ImGuiWindowFlags_MenuBar;
		// window_flags |= ImGuiWindowFlags_NoMove;
		// window_flags |= ImGuiWindowFlags_NoResize;
		// window_flags |= ImGuiWindowFlags_NoCollapse;
		// window_flags |= ImGuiWindowFlags_NoNav;
		// window_flags |= ImGuiWindowFlags_NoBackground;
		// window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
		// window_flags |= ImGuiWindowFlags_UnsavedDocument;

		if (!ImGui::Begin("LEI 3D", p_open, window_flags))
		{
			// Early out if the window is collapsed, as an optimization.
			ImGui::End();
			return;
		}

		// Menu Bar
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Menu"))
			{
				ImGui::MenuItem("Show Demo Window", NULL, &m_ShowDemoWindow);

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::Text("PRESS TAB TO TOGGLE EDITOR");
		ImGui::Text("HOLD SHIFT TO MOVE FASTER WITH CAMERA");

		Scene&	   scene = SceneManager::ActiveScene();
		SceneView& view = Application::GetSceneView();

		//FLY CAMERA ---------------------------------------------
		view.OnImGuiRender(scene);

		if (ImGui::CollapsingHeader("Scenes"))
		{
			static int selectedScene = 0;
			int		   sceneI = 0;
			for (std::string& name : SceneManager::GetSceneNames())
			{
				const int oldSelectedScene = selectedScene;
				if (ImGui::RadioButton(name.c_str(), &selectedScene, sceneI))
				{
					if (selectedScene != oldSelectedScene)
					{
						SceneManager::SetScene(name);
					}
				}
				sceneI++;
			}
		}

		if (ImGui::CollapsingHeader("Game Info"))
		{
			ImGui::Text("fps = %f", 1.0f / Application::DeltaTime());
		}

		ImGui::SetWindowSize(ImVec2(400, 400), ImGuiCond_Once);
		ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_Once);
		ImGui::End();

		scene.ShowHeirarchyGUI();
	}

	// SUPER JANKY AND SCUFFED! REDO AFTER 2D LINE RENDERING IS IMPLEMENTED
	void EditorGUI::renderDebugCursor()
	{
		PrimitiveRenderer& lineRenderer = Application::GetPrimitiveRenderer();
		Camera& camera = Application::GetSceneCamera();
		glm::vec3 origin = camera.GetPosition() + camera.GetFront() * 10.f;
		lineRenderer.pushLine(Application::GetSceneCamera(), origin, origin + glm::vec3{1,0,0}, glm::vec3{1,0,0}, .01);
		lineRenderer.pushLine(Application::GetSceneCamera(), origin, origin + glm::vec3{0,1,0}, glm::vec3{0,1,0}, .01);
		lineRenderer.pushLine(Application::GetSceneCamera(), origin, origin + glm::vec3{0,0,1}, glm::vec3{0,0,1}, .01);
	}
} // namespace lei3d
