#pragma once

#include "core/Scene.hpp"

namespace lei3d
{
	class Scene;

	//Creates a view into a scene based on editor context.
	//This seems kind of empty, but we might expand on it later, so it's good to abstract now. 
	class SceneView
	{
	public:
		enum Mode
		{
			ModeScene,
			ModeGame
		};
	private:
		//I chose not to hold a reference to the scene here and have it as an argument to functions 
		//since syncing the scene with SceneManager etc. is a pain and not necessary.
		Mode   m_Mode;

	public:
		SceneView();
		~SceneView();

		void   SetMode(Mode mode);
		void TogglePlayPause(Scene& scene);

		Mode GetMode() const;

		Camera& ActiveCamera(const Scene& scene) const;
	};
} // namespace lei3d