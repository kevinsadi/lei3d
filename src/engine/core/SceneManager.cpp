#include "SceneManager.hpp"

#include "scenes/EmptyScene.hpp"
#include "scenes/TestSceneKevin.hpp"
#include "scenes/TestSceneLogan.hpp"

#include "util/StringUtil.hpp"

namespace lei3d
{

	/* Need to implement these function pointers manually in TestScene.hpp / TestScene.cpp
	 * bc you can't cast from std::unique_ptr<TestBlaBlaScene> (*) () to std::unique_ptr<Scene> (*) ()
	 * so we do this stinky boiler plate instead
	 * */
	std::unordered_map<std::string, std::unique_ptr<Scene> (*)()> SceneManager::s_SceneConstructors = {
		{ "Test Kevin", MakeTestSceneKevin },
		{ "Test Logan", MakeTestSceneLogan },
	};

	SceneManager::SceneManager()
	{
	}

	SceneManager& SceneManager::GetInstance()
	{
		static SceneManager instance;
		return instance;
	}

	void SceneManager::Init()
	{
		BuildScenesFromFile("data/config/Build.config");
	}

	void SceneManager::BuildScenesFromFile(std::string filepath)
	{
		LEI_INFO("LOADING SCENES");
		std::ifstream fileStream(filepath, std::ifstream::in);

		std::string line;

		while (fileStream.good() && std::getline(fileStream, line))
		{
			LEI_INFO(line);

			// auto tokens = tokenize(line, ' ');
			// if (tokens.empty())
			//{
			//     continue;
			// }

			std::string sceneName = strTrim(line);

			LEI_INFO(sceneName);

			if (!sceneName.empty())
			{
				m_AllScenes.push_back({ sceneName, s_SceneConstructors[sceneName]() });
			}
		}
	}

	bool SceneManager::HasScenes()
	{
		return !GetInstance().m_AllScenes.empty();
	}

	void SceneManager::SetScene(int sceneIndex)
	{
		const auto& scenes = GetInstance().m_AllScenes;

		if (sceneIndex < 0 || sceneIndex >= scenes.size())
		{
			LEI_ERROR("Tried to load scene with out of bounds index.");
			return;
		}

		Scene* nextScene = scenes[sceneIndex].second.get();
		GetInstance().SetNextScene(*nextScene);
	}

	void SceneManager::SetScene(std::string sceneName)
	{
		// This is a structured binding. It allows us to unpack the scene name and object from an std::pair implicitly.
		for (const auto& [currName, currScene] : GetInstance().m_AllScenes)
		{
			if (sceneName == currName)
			{
				GetInstance().SetNextScene(*currScene);
				return;
			}
		}

		LEI_ERROR("DID NOT FIND SCENE: {0}", sceneName);
	}

	std::vector<std::string> SceneManager::GetSceneNames()
	{
		std::vector<std::string> names;
		for (const auto& [name, scene] : GetInstance().m_AllScenes)
		{
			names.push_back(name);
		}

		return names;
	}

	void SceneManager::SetNextScene(Scene& scene)
	{
		m_NextScene = &scene;
		m_NeedsSceneSwitch = true;
	}

	Scene& SceneManager::ActiveScene()
	{
		LEI_ASSERT(m_ActiveScene, "Attempt to access active scene when there wasn't one.");
		return *(GetInstance().m_ActiveScene);
	}

	void SceneManager::LoadNextScene()
	{
		if (m_NextScene == nullptr)
		{
			LEI_ERROR("Cannot load next scene because a scene was not set.");
			return;
		}

		LoadScene(*m_NextScene);
		m_NeedsSceneSwitch = false;
	}

	bool SceneManager::NeedsSceneSwitch() const
	{
		return m_NeedsSceneSwitch;
	}

	void SceneManager::LoadScene(Scene& scene)
	{
		if (m_ActiveScene != nullptr)
		{
			m_ActiveScene->Unload();
		}

		m_ActiveScene = &scene;
		m_ActiveScene->Load();
	}

} // namespace lei3d