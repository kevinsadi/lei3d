#include "SceneIntro.hpp"

#include "core/Application.hpp"

#include "components/FollowCameraController.hpp"
#include "components/ModelInstance.hpp"
#include "components/ColorSource.hpp"

#include "logging/GLDebug.hpp"

#include "physics/PhysicsWorld.hpp"

#include <glm/glm.hpp>

namespace lei3d
{
	std::unique_ptr<Scene> MakeSceneIntro()
	{
		return std::make_unique<SceneIntro>();
	}

	SceneIntro::SceneIntro()
	{
	}

	SceneIntro::~SceneIntro()
	{
	}

	void SceneIntro::OnLoad()
	{
		// load textures
		stbi_set_flip_vertically_on_load(true);

		// Load Models
		const std::string backpackPath = "data/models/backpack/backpack.obj";
		if (backpackModel)
		{
			backpackModel.reset();
		}
		backpackModel = std::make_unique<Model>(backpackPath);

		const std::string physicsPlaygroundPath = "data/models/leveldesign/KevWorldColorFive.obj";
		if (playgroundModel)
		{
			playgroundModel.reset();
		}
		playgroundModel = std::make_unique<Model>(physicsPlaygroundPath);

		for (std::unique_ptr<Model>& model : m_EnviromentModels)
		{
			if (model)
			{
				model.reset();
			}
		}

		// this will break as you load multiple times, it will make multiple models
		const std::string flowerPath = "data/models/leveldesignobj/flower/flower_export_2.obj";
		m_EnviromentModels.emplace_back(std::make_unique<Model>(flowerPath));

		// BACKPACK (Character) ---------------------
		Entity& backpackObj = AddEntity("Backpack");

		// ModelInstance* modelRender = backpackObj.AddComponent<ModelInstance>();
		// modelRender->Init(backpackModel.get());
		backpackObj.SetScale(glm::vec3(1.f, 1.f, 1.f));
		backpackObj.SetPosition(glm::vec3(0.f, 60.f, 0.f));
		backpackObj.SetYawRotation(0);

		CharacterController* characterController = backpackObj.AddComponent<CharacterController>();
		characterController->Init(1.f, 3.f);

		FollowCameraController* followCam = backpackObj.AddComponent<FollowCameraController>();
		followCam->Init(*m_DefaultCamera, glm::vec3(0.0f, 1.0f, 0.0f));

		// PHYSICS PLAYGROUND---------------------
		Entity& physicsPlaygroundObj = AddEntity("Physics Playground");
		physicsPlaygroundObj.SetYawRotation(0);

		ModelInstance* playgroundRender = physicsPlaygroundObj.AddComponent<ModelInstance>();
		playgroundRender->Init(playgroundModel.get());
		physicsPlaygroundObj.SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
		physicsPlaygroundObj.SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));

		StaticCollider* physicsPlaygroundCollider = physicsPlaygroundObj.AddComponent<StaticCollider>();
		physicsPlaygroundCollider->Init();
		physicsPlaygroundCollider->SetColliderToModel(*playgroundModel);

		// Flower ---------------------
		Entity& flowerObj = AddEntity("Flower");
		flowerObj.SetYawRotation(0);

		ModelInstance* flowerRender = flowerObj.AddComponent<ModelInstance>();
		flowerRender->Init(m_EnviromentModels[0].get());
		flowerObj.SetScale(glm::vec3(2.5f, 2.5f, 2.5f));
		flowerObj.SetPosition(glm::vec3(0.0f, 50.3f, 0.0f));

		// color source
		Entity& startColorSrcObj = AddEntity("Start Color Area");
		ColorSource* startSrc = startColorSrcObj.AddComponent<ColorSource>();
		startSrc->Init(5, 10, true);
		startColorSrcObj.SetPosition(glm::vec3(0, 0, 0));

		////Test Multiple Components
		Entity& skyboxObj = AddEntity("Skybox");

		SkyBox* skybox = skyboxObj.AddComponent<SkyBox>();
		std::vector<std::string> faces{ "data/skybox/anime_etheria/right.jpg", "data/skybox/anime_etheria/left.jpg",
			"data/skybox/anime_etheria/up.jpg", "data/skybox/anime_etheria/down.jpg",
			"data/skybox/anime_etheria/front.jpg", "data/skybox/anime_etheria/back.jpg" };
		skybox->Init(faces);

		AudioPlayer::PlaySFX("win.mp3");
	}

	void SceneIntro::OnReset()
	{
		// Just need to reset the backpack.
		Entity* backpackObj = GetEntity("Backpack");
		backpackObj->SetScale(glm::vec3(1.f, 1.f, 1.f));
		backpackObj->SetPosition(glm::vec3(0.f, 60.f, 0.f));

		Entity* colorObj = GetEntity("Start Color Area");
		colorObj->GetComponent<ColorSource>()->radius = 0;

		AudioPlayer::PlaySFX("win.mp3");
	}

	void SceneIntro::OnUpdate()
	{
		Entity* colorObj = GetEntity("Start Color Area");
		colorObj->GetComponent<ColorSource>()->radius += 0.8;
	}

	void SceneIntro::OnPhysicsUpdate()
	{
		m_PhysicsWorld->Step(Application::DeltaTime());
	}
} // namespace lei3d