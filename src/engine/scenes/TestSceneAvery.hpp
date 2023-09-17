#pragma once

#include "core/Scene.hpp"

namespace lei3d
{
	class Model;

	class TestSceneAvery : public Scene
	{
	public:
		TestSceneAvery();
		~TestSceneAvery();

		void OnLoad() override;
		void OnUpdate() override;
		void OnPhysicsUpdate() override;

	private:
		std::unique_ptr<Model> backpackModel;
		std::unique_ptr<Model> playgroundModel;
	};

	extern std::unique_ptr<Scene> MakeTestSceneAvery();
} // namespace lei3d