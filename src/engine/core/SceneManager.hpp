#pragma once

#include "Scene.hpp"

#include <memory>
#include <vector>

namespace lei3d
{
	class Scene;

	class SceneManager
	{
	private:
		static SceneManager*												 s_Instance;
		static std::unordered_map<std::string, std::unique_ptr<Scene> (*)()> s_SceneConstructors;

		std::vector<std::pair<std::string, std::unique_ptr<Scene>>> m_AllScenes;
		Scene*														m_ActiveScene = nullptr;
		Scene*														m_NextScene = nullptr;
		bool														m_NeedsSceneSwitch = false;

	public:
		SceneManager();

		static void SetScene(int sceneIndex);
		static void SetScene(std::string sceneName);

		static Scene&					ActiveScene();
		static std::vector<std::string> GetSceneNames();

		static bool HasScenes();

		void Init();
		void LoadNextScene();
		bool NeedsSceneSwitch() const;

		void BuildScenesFromFile(std::string filepath);

	private:
		void SetNextScene(Scene& scene);
		void LoadScene(Scene& scene);
	};
} // namespace lei3d