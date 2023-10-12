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

		std::vector<std::unique_ptr<Model>> m_EnviromentModels;

		//void Initialize
	};

	extern std::unique_ptr<Scene> MakeTestSceneKevin();
} // namespace lei3d