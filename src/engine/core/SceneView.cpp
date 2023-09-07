#include "SceneView.hpp"

#include "core/Application.hpp"

#include <imgui.h>

namespace lei3d {
	SceneView::SceneView()
	{
		// load camera
		GLFWwindow* const win = Application::Window();

		// Add Default Fly Camera
		m_FlyCamera = std::make_unique<FlyCamera>(win, 90.0f, 0.0f);
	}

	SceneView::~SceneView()
	{
		//lmfao
	}

	SceneView::Mode SceneView::GetMode() const
	{
		return m_Mode;
	}

	void SceneView::SetMode(Mode mode)
	{
		m_Mode = mode;
	}

	void SceneView::TogglePlayPause(Scene& scene)
	{
		if (m_Mode == ModeGame)
		{
			SetMode(ModeScene);
			scene.Pause();
		}
		else
		{
			SetMode(ModeGame);
			scene.Play();
		}
	}

	Camera& SceneView::ActiveCamera(const Scene& scene) const
	{
		switch (m_Mode)
		{
			case Mode::ModeScene:
				return *m_FlyCamera;
			case Mode::ModeGame:
				return scene.GetMainCamera();
		}
	}

	void SceneView::OnImGuiRender(Scene& scene)
	{
		// SCENE CONTROL WIDGETS ----------------------------------------------
		
		std::stringstream ss;
		ss << "State: ";
		ss << scene.StateToString();
		ImGui::Text(ss.str().c_str());

		if (ImGui::Button("Play"))
		{
			TogglePlayPause(scene);
		}

		ImGui::SameLine();
		if (ImGui::Button("Pause"))
		{
			SetMode(SceneView::ModeScene);
			scene.Pause();
		}

		ImGui::SameLine();
		if (ImGui::Button("Reset"))
		{
			SetMode(SceneView::ModeScene);
			scene.Reset();
		}

		ImGui::Text("Fly Camera: ");
		m_FlyCamera->OnImGuiRender();
	}
}