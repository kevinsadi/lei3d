#include "SceneView.hpp"

namespace lei3d {
	SceneView::SceneView()
	{
		//lmao
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
				return scene.GetFlyCamera();
			case Mode::ModeGame:
				return scene.GetMainCamera();
		}
	}
}