#include "SceneIntro.hpp"

#include "core/Application.hpp"
#include "rendering/gui/GuiManager.hpp"
#include "rendering/gui/screens/BaseGuiScreen.hpp"
#include "rendering/gui/screens/GuiScreen.hpp"

#include "components/FollowCameraController.hpp"
#include "components/ModelInstance.hpp"
#include "components/ColorSource.hpp"
#include "components/LevelSwitchCollider.hpp"

#include "logging/GLDebug.hpp"

#include "physics/PhysicsWorld.hpp"

#include <glm/glm.hpp>

#include <algorithm>

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

		// Flowers ---------------------
		Entity& flowerObj1 = AddEntity("Flower 1");
		flowerObj1.SetYawRotation(0);
		ModelInstance* flowerRender = flowerObj1.AddComponent<ModelInstance>();
		flowerRender->Init(m_EnviromentModels["flower"].get());
		flowerObj1.SetScale(glm::vec3(2.5f, 2.5f, 2.5f));
		flowerObj1.SetPosition(glm::vec3(0.0f, 51.3f, 0.0f));
		ColorSource* startSrc = flowerObj1.AddComponent<ColorSource>();
		startSrc->Init(5, 10, true);
		StaticCollider* flowerCollider1 = flowerObj1.AddComponent<StaticCollider>();
		flowerCollider1->Init();
		flowerCollider1->SetColliderToModel(*m_EnviromentModels["flower"].get());
		PlayerTriggerComponent* playerTrigger1 = flowerObj1.AddComponent<PlayerTriggerComponent>();
		playerTrigger1->SetOnPlayerEntered([&]() {
			if (!color_1)
			{
				color_1 = true;
				// this is a code smell, but I cannot think of a cleaner way to do this
				dynamic_cast<BaseGuiScreen&>(GuiManager::Instance().GetBaseScreen()).AddFlower();
				AudioPlayer::GetInstance().PlaySFX("collect.mp3");
			}
		});

		Entity& flowerObj2 = AddEntity("Flower 2");
		flowerObj2.SetYawRotation(60);
		ModelInstance* flowerRender2 = flowerObj2.AddComponent<ModelInstance>();
		flowerRender2->Init(m_EnviromentModels["flower"].get());
		flowerObj2.SetScale(glm::vec3(2.5f, 2.5f, 2.5f));
		flowerObj2.SetPosition(glm::vec3(5.0f, 8.8f, -147.0f));
		ColorSource* startSrc2 = flowerObj2.AddComponent<ColorSource>();
		startSrc2->Init(5, 10, true);
		StaticCollider* flowerCollider2 = flowerObj2.AddComponent<StaticCollider>();
		flowerCollider2->Init();
		flowerCollider2->SetColliderToModel(*m_EnviromentModels["flower"].get());
		PlayerTriggerComponent* playerTrigger2 = flowerObj2.AddComponent<PlayerTriggerComponent>();
		playerTrigger2->SetOnPlayerEntered([&]() {
			if (!color_2)
			{
				color_2 = true;
				// this is a code smell, but I cannot think of a cleaner way to do this
				dynamic_cast<BaseGuiScreen&>(GuiManager::Instance().GetBaseScreen()).AddFlower();
				AudioPlayer::GetInstance().PlaySFX("collect.mp3");
			}
		});

		Entity& flowerObj3 = AddEntity("Flower 3");
		flowerObj3.SetYawRotation(0);
		ModelInstance* flowerRender3 = flowerObj3.AddComponent<ModelInstance>();
		flowerRender3->Init(m_EnviromentModels["flower"].get());
		flowerObj3.SetScale(glm::vec3(2.5f, 2.5f, 2.5f));
		flowerObj3.SetPosition(glm::vec3(51.5f, -10.6f, -42.0f));
		ColorSource* startSrc3 = flowerObj3.AddComponent<ColorSource>();
		startSrc3->Init(5, 10, true);
		StaticCollider* flowerCollider3 = flowerObj3.AddComponent<StaticCollider>();
		flowerCollider3->Init();
		flowerCollider3->SetColliderToModel(*m_EnviromentModels["flower"].get());
		PlayerTriggerComponent* playerTrigger3 = flowerObj3.AddComponent<PlayerTriggerComponent>();
		playerTrigger3->SetOnPlayerEntered([&]() {
			if (!color_3)
			{
				color_3 = true;
				// this is a code smell, but I cannot think of a cleaner way to do this
				dynamic_cast<BaseGuiScreen&>(GuiManager::Instance().GetBaseScreen()).AddFlower();
				AudioPlayer::GetInstance().PlaySFX("collect.mp3");
			}
		});

		// Final Color Source ------------------------------
		Entity& endColorSrcObj = AddEntity("endColor");
		ColorSource* endSrc = endColorSrcObj.AddComponent<ColorSource>();
		endSrc->Init(5, 10, false);
		endColorSrcObj.SetPosition(glm::vec3(0, 0, 0));
		TimerComponent* timerComponent = endColorSrcObj.AddComponent<TimerComponent>();
		timerComponent->SetTargetTime(9.0f);
		timerComponent->OnTimerEnd([&]() {
			LEI_TRACE("Timer Ended!!!");
			// this is still bad
			AudioPlayer::GetInstance().PlaySFX("menu_reverse.mp3");
			SceneManager::SetScene("Test Kevin");
			dynamic_cast<BaseGuiScreen&>(GuiManager::Instance().GetBaseScreen()).ResetFlowers();
			dynamic_cast<BaseGuiScreen&>(GuiManager::Instance().GetBaseScreen()).ResetTimer();
		});

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
		fishObj.SetScale(glm::vec3(10.0f, 10.0f, 10.0f));
		fishObj.SetPosition(glm::vec3(36.0f, 17.0f, -6.5f));
		fishObj.SetYawRotation(0);

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

		////Test Multiple Components
		Entity& skyboxObj = AddEntity("Skybox");

		SkyBox* skybox = skyboxObj.AddComponent<SkyBox>();
		std::vector<std::string> faces{ "data/skybox/anime_etheria/right.jpg", "data/skybox/anime_etheria/left.jpg",
			"data/skybox/anime_etheria/up.jpg", "data/skybox/anime_etheria/down.jpg",
			"data/skybox/anime_etheria/front.jpg", "data/skybox/anime_etheria/back.jpg" };
		skybox->Init(faces);

		dynamic_cast<BaseGuiScreen&>(GuiManager::Instance().GetBaseScreen()).ResetTimer();
		// AudioPlayer::PlaySFX("win.mp3");
	}

	void SceneIntro::OnReset()
	{
		// Just need to reset the backpack.
		Entity* backpackObj = GetEntity("Backpack");
		backpackObj->SetScale(glm::vec3(1.f, 1.f, 1.f));
		backpackObj->SetPosition(glm::vec3(0.f, 60.f, 0.f));

		AudioPlayer::GetInstance().PlaySFX("reset.mp3");
	}

	void SceneIntro::OnUpdate()
	{
		float deltaColorRadius = Application::DeltaTime() * 50.0f;

		if (color_1)
		{
			Entity* colorObj1 = GetEntity("Flower 1");
			colorObj1->GetComponent<ColorSource>()->radius = std::clamp(colorObj1->GetComponent<ColorSource>()->radius + deltaColorRadius, 0.0f, 20.0f);
		}
		if (color_2)
		{
			Entity* colorObj2 = GetEntity("Flower 2");
			colorObj2->GetComponent<ColorSource>()->radius = std::clamp(colorObj2->GetComponent<ColorSource>()->radius + deltaColorRadius, 0.0f, 100.0f);
		}
		if (color_3)
		{
			Entity* colorObj3 = GetEntity("Flower 3");
			colorObj3->GetComponent<ColorSource>()->radius = std::clamp(colorObj3->GetComponent<ColorSource>()->radius + deltaColorRadius, 0.0f, 100.0f);
		}

		if (color_1 && color_2 && color_3)
		{
			Entity* colorObj4 = GetEntity("endColor");
			// AudioPlayer::GetInstance().PlaySFX("win.mp3");
			colorObj4->GetComponent<ColorSource>()->SetActive(true);
			colorObj4->GetComponent<ColorSource>()->radius += deltaColorRadius * 2;
			if (!m_Won)
			{
				m_Won = true;
				colorObj4->GetComponent<TimerComponent>()->StartTimer();
			}
		}
	}

	void SceneIntro::OnPhysicsUpdate()
	{
		m_PhysicsWorld->Step(Application::DeltaTime());
	}
} // namespace lei3d