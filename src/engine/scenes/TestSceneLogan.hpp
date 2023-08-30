#pragma once

#include "components/CharacterController.hpp"
#include "components/SkyBox.hpp"
#include "components/StaticCollider.hpp"
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

	class TestSceneLogan : public Scene
	{
	public:
		TestSceneLogan();
		~TestSceneLogan();

		void OnLoad() override;
		void OnPhysicsUpdate() override;

	private:
		std::unique_ptr<Model> backpackModel;
		std::unique_ptr<Model> playgroundModel;
	};

} // namespace lei3d