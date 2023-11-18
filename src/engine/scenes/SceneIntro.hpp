#pragma once

#include "components/CharacterController.hpp"
#include "components/SkyBox.hpp"
#include "components/StaticCollider.hpp"
#include "components/PlayerTriggerComponent.hpp"
#include "core/Entity.hpp"
#include "core/Scene.hpp"
#include "physics/PhysicsWorld.hpp"
#include "rendering/Shader.hpp"

namespace lei3d
{

	class Entity;
	class Model;
	class Shader;
	class SkyBox;
	class PhysicsWorld;

	class SceneIntro : public Scene
	{
	public:
		SceneIntro();
		~SceneIntro();

		void OnLoad() override;
		void OnPhysicsUpdate() override;
		void OnUpdate() override;
		void OnReset() override;

	private:
		std::unique_ptr<Model> backpackModel;
		std::unique_ptr<Model> playgroundModel;
		std::unordered_map<std::string, std::unique_ptr<Model>> m_EnviromentModels;
		bool color_1 = false;
		bool color_2 = false;
		bool color_3 = false;
		bool m_Won = false;
	};

	extern std::unique_ptr<Scene> MakeSceneIntro();
} // namespace lei3d