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
		backpackObj.SetPosition(glm::vec3(0.f, 26.f, 35.f));
		backpackObj.SetYawRotation(90);

		// CharacterController* characterController = backpackObj.AddComponent<CharacterController>();
		// characterController->Init(1.f, 3.f);

		FollowCameraController* followCam = backpackObj.AddComponent<FollowCameraController>();
		followCam->Init(*m_DefaultCamera, glm::vec3(0.0f, 5.0f, 0.0f));

		backpackObj.SetYawRotation(0);

		// PHYSICS PLAYGROUND---------------------
		Entity& physicsPlaygroundObj = AddEntity("Physics Playground");
		physicsPlaygroundObj.SetYawRotation(0);

		ModelInstance* playgroundRender = physicsPlaygroundObj.AddComponent<ModelInstance>();
		playgroundRender->Init(playgroundModel.get());
		physicsPlaygroundObj.SetScale(glm::vec3(15.f, 5.f, 10.0f));
		physicsPlaygroundObj.SetPosition(glm::vec3(25.0f, 10.0f, 39.0f));

		StaticCollider* physicsPlaygroundCollider = physicsPlaygroundObj.AddComponent<StaticCollider>();
		physicsPlaygroundCollider->Init();
		physicsPlaygroundCollider->SetColliderToModel(*playgroundModel);

		// Flower ---------------------
		Entity& flowerObj = AddEntity("Flower");
		flowerObj.SetYawRotation(0);

		ModelInstance* flowerRender = flowerObj.AddComponent<ModelInstance>();
		flowerRender->Init(m_EnviromentModels["flower"].get());
		flowerObj.SetScale(glm::vec3(10.0f, 10.0f, 10.0f));
		flowerObj.SetPosition(glm::vec3(55.0f, -5.0f, -30.0f));

		// color source
		Entity& startColorSrcObj = AddEntity("Start Color Area");
		ColorSource* startSrc = startColorSrcObj.AddComponent<ColorSource>();
		startSrc->Init(5, 10, true);
		startColorSrcObj.SetPosition(glm::vec3(0, 0, 0));

		// House ---------------------
		Entity& houseObj = AddEntity("House");
		houseObj.SetScale(glm::vec3(3.0f, 4.5f, 3.0f));
		houseObj.SetPosition(glm::vec3(640.0f, 130.0f, -1220.0f));
		houseObj.SetYawRotation(0);

		ModelInstance* houseRender = houseObj.AddComponent<ModelInstance>();
		houseRender->Init(m_EnviromentModels["house"].get());

		StaticCollider* houseCollider = houseObj.AddComponent<StaticCollider>();
		houseCollider->Init();
		houseCollider->SetColliderToModel(*m_EnviromentModels["house"].get());

		// Sign ---------------------
		Entity& signObj = AddEntity("Sign");
		signObj.SetScale(glm::vec3(15.0f, 15.0f, 15.0f));
		signObj.SetPosition(glm::vec3(125.0f, -10.0f, -350.0f));
		signObj.SetYawRotation(180.0f);

		ModelInstance* signRender = signObj.AddComponent<ModelInstance>();
		signRender->Init(m_EnviromentModels["sign"].get());

		StaticCollider* signCollider = signObj.AddComponent<StaticCollider>();
		signCollider->Init();
		signCollider->SetColliderToModel(*m_EnviromentModels["sign"].get());

		// Fish ---------------------
		Entity& fishObj = AddEntity("Fish");

		m_fishTheta = 0.0f;
		m_fishRadius = glm::length(glm::vec2(400.5f, 85.0f));
		//                2 * PI radians        (14 second loop, assuming 60fps)
		m_deltaTheta = (2 * glm::pi<float>()) / (5 * 60);

		fishObj.SetScale(glm::vec3(10.0f, 10.0f, 10.0f));
		fishObj.SetPosition(glm::vec3(-50.0f, 150.0f, -400.0f));
		fishObj.SetYawRotation(-45.0f);

		ModelInstance* fishRender = fishObj.AddComponent<ModelInstance>();
		fishRender->Init(m_EnviromentModels["fish"].get());

		Entity& fishObj2 = AddEntity("Fish2");
		fishObj2.SetScale(glm::vec3(10.0f, 10.0f, 10.0f));
		fishObj2.SetPosition(glm::vec3(0.0f, 125.0f, -425.0f));
		fishObj2.SetYawRotation(-45.0f);

		ModelInstance* fishRender2 = fishObj2.AddComponent<ModelInstance>();
		fishRender2->Init(m_EnviromentModels["fish"].get());

		Entity& fishObj3 = AddEntity("Fish3");
		fishObj3.SetScale(glm::vec3(10.0f, 10.0f, 10.0f));
		fishObj3.SetPosition(glm::vec3(75.0f, 175.0f, -475.0f));
		fishObj3.SetYawRotation(-45.0f);

		ModelInstance* fishRender3 = fishObj3.AddComponent<ModelInstance>();
		fishRender3->Init(m_EnviromentModels["fish"].get());

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
		islandObj2.SetScale(glm::vec3(40.0f, 20.0f, 35.0f));
		islandObj2.SetPosition(glm::vec3(125.0f, -20.0f, -350.0f));

		ModelInstance* islandRender2 = islandObj2.AddComponent<ModelInstance>();
		islandRender2->Init(m_EnviromentModels["island"].get());

		StaticCollider* islandCollider2 = islandObj2.AddComponent<StaticCollider>();
		islandCollider2->Init();
		islandCollider2->SetColliderToModel(*m_EnviromentModels["island"].get());

		Entity& islandObj3 = AddEntity("Island3");
		islandObj3.SetScale(glm::vec3(40.0f, 30.0f, 35.0f));
		islandObj3.SetPosition(glm::vec3(450.0f, -0.0f, -750.0f));
		islandObj3.SetYawRotation(0);

		ModelInstance* islandRender3 = islandObj3.AddComponent<ModelInstance>();
		islandRender3->Init(m_EnviromentModels["island"].get());

		StaticCollider* islandCollider3 = islandObj3.AddComponent<StaticCollider>();
		islandCollider3->Init();
		islandCollider3->SetColliderToModel(*m_EnviromentModels["island"].get());

		Entity& islandObj4 = AddEntity("Island4");
		islandObj4.SetScale(glm::vec3(25.0f, 15.0f, 20.0f));
		islandObj4.SetPosition(glm::vec3(425.0f, 75.0f, -1000.0f));
		islandObj4.SetYawRotation(0);

		ModelInstance* islandRender4 = islandObj4.AddComponent<ModelInstance>();
		islandRender4->Init(m_EnviromentModels["island"].get());

		StaticCollider* islandCollider4 = islandObj4.AddComponent<StaticCollider>();
		islandCollider4->Init();
		islandCollider4->SetColliderToModel(*m_EnviromentModels["island"].get());

		Entity& islandObj5 = AddEntity("Island5");
		islandObj5.SetScale(glm::vec3(25.0f, 15.0f, 20.0f));
		islandObj5.SetPosition(glm::vec3(650.0f, 125.0f, -1250.0f));
		islandObj5.SetYawRotation(0);

		ModelInstance* islandRender5 = islandObj5.AddComponent<ModelInstance>();
		islandRender5->Init(m_EnviromentModels["island"].get());

		StaticCollider* islandCollider5 = islandObj5.AddComponent<StaticCollider>();
		islandCollider5->Init();
		islandCollider5->SetColliderToModel(*m_EnviromentModels["island"].get());

		Entity& islandObj6 = AddEntity("Island6");
		islandObj6.SetScale(glm::vec3(20.0f, 10.0f, 15.0f));
		islandObj6.SetPosition(glm::vec3(-250.0f, 75.0f, -300.0f));
		islandObj6.SetYawRotation(0);

		ModelInstance* islandRender6 = islandObj6.AddComponent<ModelInstance>();
		islandRender6->Init(m_EnviromentModels["island"].get());

		StaticCollider* islandCollider6 = islandObj6.AddComponent<StaticCollider>();
		islandCollider6->Init();
		islandCollider6->SetColliderToModel(*m_EnviromentModels["island"].get());

		// TriggerCollider* trigger = islandObj2.AddComponent<TriggerCollider>();

		// LevelSwitchCollider* triggerCollider = islandObj2.AddComponent<LevelSwitchCollider>();
		// std::vector<const btCollisionObject*> ignoredObjects;
		// ignoredObjects.push_back(characterController->getRigidBody());
		// // ignoredObjects.push_back(islandCollider2->getRigidBody());
		// triggerCollider->Init(characterController->getGroundCheckObj(), ignoredObjects);

		////Test Multiple Components
		Entity& skyboxObj = AddEntity("Skybox");

		SkyBox* skybox = skyboxObj.AddComponent<SkyBox>();
		std::vector<std::string> faces{ "data/skybox/jessica_sky/right.jpg", "data/skybox/jessica_sky/left.jpg",
			"data/skybox/jessica_sky/py.jpg", "data/skybox/jessica_sky/down.jpg",
			"data/skybox/jessica_sky/front.jpg", "data/skybox/jessica_sky/back.jpg" };
		skybox->Init(faces);

		AudioPlayer::GetInstance().PlayMusic("skylei_title.mp3", 0.4f);
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
		colorObj->GetComponent<ColorSource>()->radius += 60 * Application::GetInstance().DeltaTime();

		// Fly fishy fly!
		m_fishTheta -= m_deltaTheta;
		Entity* fishObj = GetEntity("Fish");
		fishObj->SetPosition(glm::vec3(
			m_fishRadius * glm::cos(m_fishTheta),
			150.0f,
			m_fishRadius * glm::sin(m_fishTheta)));
		fishObj->SetYawRotation(m_fishTheta * -360.0f / (2.0f * glm::pi<float>()));
	}

	void SceneTitle::OnPhysicsUpdate()
	{
		m_PhysicsWorld->Step(Application::DeltaTime());
	}
} // namespace lei3d