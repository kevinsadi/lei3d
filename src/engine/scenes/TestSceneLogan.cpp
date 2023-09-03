#include "TestSceneLogan.hpp"

#include "core/Application.hpp"
#include "components/ModelInstance.hpp"
#include "logging/GLDebug.hpp"
#include "physics/PhysicsWorld.hpp"

#include <glm/glm.hpp>

namespace lei3d
{
	std::unique_ptr<Scene> MakeTestSceneLogan()
	{
		return std::make_unique<TestSceneLogan>();
	}

	TestSceneLogan::TestSceneLogan()
	{
	}

	TestSceneLogan::~TestSceneLogan()
	{
	}

	void TestSceneLogan::OnLoad()
	{
		m_Camera->SetPosition({ 0.f, 80.f, -60.f });
		// load textures
		stbi_set_flip_vertically_on_load(true);

		// Load Models
		const std::string backpackPath = "data/models/backpack/backpack.obj";
		backpackModel = std::make_unique<Model>(backpackPath);
		const std::string physicsPlaygroundPath = "data/models/leveldesign/KevWorldColorFive.obj";
		playgroundModel = std::make_unique<Model>(physicsPlaygroundPath);

		// BACKPACK (Character) ---------------------
		Entity& backpackObj = AddEntity("Backpack");

		ModelInstance* modelRender = backpackObj.AddComponent<ModelInstance>();
		modelRender->Init(backpackModel.get());
		backpackObj.SetScale(glm::vec3(1.f, 1.f, 1.f));
		backpackObj.SetPosition(glm::vec3(0.f, 200.f, 0.f));

		CharacterController* characterController = backpackObj.AddComponent<CharacterController>();
		characterController->Init(1.f, 3.f);

		// PHYSICS PLAYGROUND---------------------
		Entity& physicsPlaygroundObj = AddEntity("Physics Playground");

		ModelInstance* playgroundRender = physicsPlaygroundObj.AddComponent<ModelInstance>();
		playgroundRender->Init(playgroundModel.get());
		physicsPlaygroundObj.SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
		physicsPlaygroundObj.SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));

		StaticCollider* physicsPlaygroundCollider = physicsPlaygroundObj.AddComponent<StaticCollider>();
		physicsPlaygroundCollider->Init();
		physicsPlaygroundCollider->SetColliderToModel(*playgroundModel);

		////Test Multiple Components
		Entity& skyboxObj = AddEntity("Skybox");

		SkyBox*					 skybox = skyboxObj.AddComponent<SkyBox>();
		std::vector<std::string> faces{ "data/skybox/anime_etheria/right.jpg", "data/skybox/anime_etheria/left.jpg",
			"data/skybox/anime_etheria/up.jpg", "data/skybox/anime_etheria/down.jpg",
			"data/skybox/anime_etheria/front.jpg", "data/skybox/anime_etheria/back.jpg" };
		skybox->Init(faces);
	}

	void TestSceneLogan::OnPhysicsUpdate()
	{
		m_PhysicsWorld->Step(Application::DeltaTime());
	}

} // namespace lei3d