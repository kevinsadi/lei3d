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

		if (ImGui::CollapsingHeader("Shortcuts/Keybinds"))
		{
			ImGui::Text("TAB: TOGGLE EDITOR");
			ImGui::Text("SHIFT: Move Faster With Camera");
			ImGui::Text("CTRL: Increase Object Transform Step");
			ImGui::Text("R: Reset the Scene");
			ImGui::Text("Q: Play/Pause Scene");
		}

		ImGui::SetWindowSize(ImVec2(400, 400), ImGuiCond_Once);
		ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_Once);
		ImGui::End();

		scene.ShowHeirarchyGUI();
	}

} // namespace lei3d