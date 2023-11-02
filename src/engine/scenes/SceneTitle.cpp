#include "SceneTitle.hpp"

#include "core/Application.hpp"

#include "components/FollowCameraController.hpp"
#include "components/ModelInstance.hpp"
#include "components/ColorSource.hpp"
#include "components/LevelSwitchCollider.hpp"

#include "logging/GLDebug.hpp"

#include "physics/PhysicsWorld.hpp"

#include <glm/glm.hpp>

namespace lei3d
{
	std::unique_ptr<Scene> MakeSceneTitle()
	{
		return std::make_unique<SceneTitle>();
	}

	SceneTitle::SceneTitle()
	{
	}

	SceneTitle::~SceneTitle()
	{
	}

	void SceneTitle::OnLoad()
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

		const std::string physicsPlaygroundPath = "data/models/leveldesignobj/cloud/cloud.obj";
		if (playgroundModel)
		{
			playgroundModel.reset();
		}
		playgroundModel = std::make_unique<Model>(physicsPlaygroundPath);

		// since this would just overwrite
		const std::string flowerPath = "data/models/leveldesignobj/flower/flower_export_2.obj";
		m_EnviromentModels.insert(std::make_pair("flower", std::make_unique<Model>(flowerPath)));

		const std::string islandPath = "data/models/leveldesignobj/island/island.obj";
		m_EnviromentModels.insert(std::make_pair("island", std::make_unique<Model>(islandPath)));

		const std::string housePath = "data/models/leveldesignobj/house/house_texture.obj";
		m_EnviromentModels.insert(std::make_pair("house", std::make_unique<Model>(housePath)));

		const std::string signPath = "data/models/leveldesignobj/sign/sign.obj";
		m_EnviromentModels.insert(std::make_pair("sign", std::make_unique<Model>(signPath)));

		const std::string fishPath = "data/models/leveldesignobj/fish2/fish.obj";
		m_EnviromentModels["fish"] = (std::make_unique<Model>(fishPath));
		m_EnviromentModels.insert(std::make_pair("fish", std::make_unique<Model>(fishPath)));

		// this is some of the worst code I have written in my life, please forgive me

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
		followCam->Init(*m_DefaultCamera, glm::vec3(0.0f, 5.0f, 0.0f));

		// PHYSICS PLAYGROUND---------------------
		Entity& physicsPlaygroundObj = AddEntity("Physics Playground");
		physicsPlaygroundObj.SetYawRotation(0);

		ModelInstance* playgroundRender = physicsPlaygroundObj.AddComponent<ModelInstance>();
		playgroundRender->Init(playgroundModel.get());
		physicsPlaygroundObj.SetScale(glm::vec3(15.f, 5.f, 10.0f));
		physicsPlaygroundObj.SetPosition(glm::vec3(0.0f, 50.0f, 0.0f));

		StaticCollider* physicsPlaygroundCollider = physicsPlaygroundObj.AddComponent<StaticCollider>();
		physicsPlaygroundCollider->Init();
		physicsPlaygroundCollider->SetColliderToModel(*playgroundModel);

		// Flower ---------------------
		Entity& flowerObj = AddEntity("Flower");
		flowerObj.SetYawRotation(0);

		ModelInstance* flowerRender = flowerObj.AddComponent<ModelInstance>();
		flowerRender->Init(m_EnviromentModels["flower"].get());
		flowerObj.SetScale(glm::vec3(2.5f, 2.5f, 2.5f));
		flowerObj.SetPosition(glm::vec3(0.0f, 50.3f, 0.0f));

		// color source
		Entity& startColorSrcObj = AddEntity("Start Color Area");
		ColorSource* startSrc = startColorSrcObj.AddComponent<ColorSource>();
		startSrc->Init(5, 10, true);
		startColorSrcObj.SetPosition(glm::vec3(0, 0, 0));

		// House ---------------------
		Entity& houseObj = AddEntity("House");
		houseObj.SetScale(glm::vec3(4.0f, 6.0f, 4.0f));
		houseObj.SetPosition(glm::vec3(-10.00f, -12.0f, -10.0f));
		houseObj.SetYawRotation(0);

		ModelInstance* houseRender = houseObj.AddComponent<ModelInstance>();
		houseRender->Init(m_EnviromentModels["house"].get());

		StaticCollider* houseCollider = houseObj.AddComponent<StaticCollider>();
		houseCollider->Init();
		houseCollider->SetColliderToModel(*m_EnviromentModels["house"].get());

		// Sign ---------------------
		Entity& signObj = AddEntity("Sign");
		signObj.SetScale(glm::vec3(3.0f, 3.0f, 3.0f));
		signObj.SetPosition(glm::vec3(13.0f, 51.0f, -7.0f));
		signObj.SetYawRotation(-70);

		ModelInstance* signRender = signObj.AddComponent<ModelInstance>();
		signRender->Init(m_EnviromentModels["sign"].get());

		StaticCollider* signCollider = signObj.AddComponent<StaticCollider>();
		signCollider->Init();
		signCollider->SetColliderToModel(*m_EnviromentModels["sign"].get());

		// Fish ---------------------
		Entity& fishObj = AddEntity("Fish");
		m_fishTheta = 0.0f;
		m_fishRadius = glm::length(glm::vec2(126.5f, 85.0f));
		//                2 * PI radians        (14 second loop, assuming 60fps)
		m_deltaTheta = (2 * glm::pi<float>()) / (14 * 60);

		fishObj.SetScale(glm::vec3(10.0f, 10.0f, 10.0f));
		fishObj.SetPosition(glm::vec3(126.5f, 85.0f, 7.0f));
		fishObj.SetYawRotation(glm::atan(126.5f, 85.0f));

		ModelInstance* fishRender = fishObj.AddComponent<ModelInstance>();
		fishRender->Init(m_EnviromentModels["fish"].get());

		// Islands ---------------------
		Entity& islandObj = AddEntity("Island");
		islandObj.SetScale(glm::vec3(30.0f, 30.0f, 30.0f));
		islandObj.SetPosition(glm::vec3(20.0f, -20.5f, -44.0f));
		islandObj.SetYawRotation(0);

		ModelInstance* islandRender = islandObj.AddComponent<ModelInstance>();
		islandRender->Init(m_EnviromentModels["island"].get());

		StaticCollider* islandCollider = islandObj.AddComponent<StaticCollider>();
		islandCollider->Init();
		islandCollider->SetColliderToModel(*m_EnviromentModels["island"].get());

		//
		Entity& islandObj2 = AddEntity("Island2");
		islandObj2.SetYawRotation(0);
		islandObj2.SetScale(glm::vec3(20.0f, 20.0f, 20.0f));
		islandObj2.SetPosition(glm::vec3(0.0f, 0.0f, -154.0f));

		ModelInstance* islandRender2 = islandObj2.AddComponent<ModelInstance>();
		islandRender2->Init(m_EnviromentModels["island"].get());

		StaticCollider* islandCollider2 = islandObj2.AddComponent<StaticCollider>();
		islandCollider2->Init();
		islandCollider2->SetColliderToModel(*m_EnviromentModels["island"].get());

		// TriggerCollider* trigger = islandObj2.AddComponent<TriggerCollider>();

		// LevelSwitchCollider* triggerCollider = islandObj2.AddComponent<LevelSwitchCollider>();
		// std::vector<const btCollisionObject*> ignoredObjects;
		// ignoredObjects.push_back(characterController->getRigidBody());
		// // ignoredObjects.push_back(islandCollider2->getRigidBody());
		// triggerCollider->Init(characterController->getGroundCheckObj(), ignoredObjects);

		////Test Multiple Components
		Entity& skyboxObj = AddEntity("Skybox");

		SkyBox* skybox = skyboxObj.AddComponent<SkyBox>();
		std::vector<std::string> faces{ "data/skybox/anime_etheria/right.jpg", "data/skybox/anime_etheria/left.jpg",
			"data/skybox/anime_etheria/up.jpg", "data/skybox/anime_etheria/down.jpg",
			"data/skybox/anime_etheria/front.jpg", "data/skybox/anime_etheria/back.jpg" };
		skybox->Init(faces);

		// AudioPlayer::PlaySFX("win.mp3");
	}

	void SceneTitle::OnReset()
	{
		// Just need to reset the backpack.
		Entity* backpackObj = GetEntity("Backpack");
		backpackObj->SetScale(glm::vec3(1.f, 1.f, 1.f));
		backpackObj->SetPosition(glm::vec3(0.f, 60.f, 0.f));

		Entity* colorObj = GetEntity("Start Color Area");
		colorObj->GetComponent<ColorSource>()->radius = 0;

		// AudioPlayer::PlaySFX("win.mp3");
	}

	void SceneTitle::OnUpdate()
	{
		Entity* colorObj = GetEntity("Start Color Area");
		colorObj->GetComponent<ColorSource>()->radius += 0.8;

		// Fly fishy fly!
		m_fishTheta -= m_deltaTheta;
		Entity* fishObj = GetEntity("Fish");
		fishObj->SetPosition(glm::vec3(
			m_fishRadius * glm::cos(m_fishTheta),
			85.0f,
			m_fishRadius * glm::sin(m_fishTheta)
		));
		fishObj->SetYawRotation(m_fishTheta * -360.0f / (2.0f * glm::pi<float>()));
	}

	void SceneTitle::OnPhysicsUpdate()
	{
		m_PhysicsWorld->Step(Application::DeltaTime());
	}
} // namespace lei3d