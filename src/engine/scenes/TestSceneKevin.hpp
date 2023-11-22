#pragma once

#include "core/Scene.hpp"

namespace lei3d
{
	class Model;

	class TestSceneKevin : public Scene
	{
	public:
		TestSceneKevin();
		~TestSceneKevin();

		void OnLoad() override;
		void OnUpdate() override;
		void OnPhysicsUpdate() override;
		void OnReset() override;

	private:
		std::unique_ptr<Model> backpackModel;
		std::unique_ptr<Model> playgroundModel;

		std::unordered_map<std::string, std::unique_ptr<Model>> m_EnviromentModels;
		bool color_1 = false;
		bool color_2 = false;
		bool color_3 = false;
		bool m_Won = false;

		// void Initialize
	};

	extern std::unique_ptr<Scene> MakeTestSceneKevin();
} // namespace lei3d