#pragma once

#include "components/CharacterController.hpp"
#include "components/SkyBox.hpp"
#include "components/StaticCollider.hpp"
#include "core/Entity.hpp"
#include "core/Scene.hpp"
#include "physics/PhysicsWorld.hpp"

#include "audio/AudioPlayer.hpp"
#include "rendering/Shader.hpp"

namespace lei3d
{
	class Entity;
	class Model;
	class Shader;
	class SkyBox;
	class PhysicsWorld;

	class TestSceneKevin : public Scene
	{
	public:
		TestSceneKevin();
		~TestSceneKevin();

		void OnLoad() override;
		void OnUpdate() override;
		void OnPhysicsUpdate() override;
		void OnRender() override;

	private:
		std::unique_ptr<Model> backpackModel;
		std::unique_ptr<Model> playgroundModel;
	};

	extern std::unique_ptr<Scene> MakeTestSceneKevin();
} // namespace lei3d