#include "TestSceneKevin.hpp"

#include "core/Application.hpp"
#include "rendering/gui/GuiManager.hpp"
#include "rendering/gui/screens/BaseGuiScreen.hpp"
#include "rendering/gui/screens/GuiScreen.hpp"

#include "components/ModelInstance.hpp"
#include "components/ColorSource.hpp"
#include "components/CharacterController.hpp"
#include "components/SkyBox.hpp"
#include "components/StaticCollider.hpp"
#include "components/FollowCameraController.hpp"
#include "components/TriggerCollider.hpp"
#include "components/TimerComponent.hpp"
#include "components/PlayerTriggerComponent.hpp"

#include "logging/GLDebug.hpp"
#include "physics/PhysicsWorld.hpp"

#include "audio/AudioPlayer.hpp"

#include <glm/glm.hpp>

namespace lei3d
{
	std::unique_ptr<Scene> MakeTestSceneKevin()
	{
		return std::make_unique<TestSceneKevin>();
	}

	TestSceneKevin::TestSceneKevin()
	{
	}

	TestSceneKevin::~TestSceneKevin()
	{
	}

	void TestSceneKevin::OnLoad()
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

		const std::string physicsPlaygroundPath = "data/models/environment/level_disassociate_2.gltf";
		// const std::string physicsPlaygroundPath = "data/models/environment/level_7_colors.gltf";

		if (playgroundModel)
		{
			playgroundModel.reset();
		}
		playgroundModel = std::make_unique<Model>(physicsPlaygroundPath);

		const std::string flowerPath = "data/models/leveldesignobj/flower/flower_export_2.obj";
		m_EnviromentModels.insert(std::make_pair("flower", std::make_unique<Model>(flowerPath)));

		const std::string islandPath = "data/models/environment/flat_island_3.gltf";
		m_EnviromentModels.insert(std::make_pair("island", std::make_unique<Model>(islandPath)));

		const std::string housePath = "data/models/leveldesignobj/house/house_texture.obj";
		m_EnviromentModels.insert(std::make_pair("house", std::make_unique<Model>(housePath)));

		const std::string fishPath = "data/models/leveldesignobj/fish2/fish.obj";
		m_EnviromentModels.insert(std::make_pair("fish", std::make_unique<Model>(fishPath)));

		const std::string treePath = "data/models/environment/palm_tree.gltf";
		m_EnviromentModels.insert(std::make_pair("tree", std::make_unique<Model>(treePath)));

		// BACKPACK (Character) ---------------------
		Entity& backpackObj = AddEntity("Backpack");

		// ModelInstance* modelRender = backpackObj.AddComponent<ModelInstance>();
		// modelRender->Init(backpackModel.get());
		backpackObj.SetScale(glm::vec3(1.f, 1.f, 1.f));
		backpackObj.SetPosition(glm::vec3(-112.5f, 505.f, 3.f));
		backpackObj.SetYawRotation(0);

		CharacterController* characterController = backpackObj.AddComponent<CharacterController>();
		characterController->Init(1.f, 3.f);

		FollowCameraController* followCam = backpackObj.AddComponent<FollowCameraController>();
		followCam->Init(*m_DefaultCamera, glm::vec3(0.0f, 1.0f, 0.0f));

		// PHYSICS PLAYGROUND---------------------
		Entity& physicsPlaygroundObj = AddEntity("Physics Playground");

		ModelInstance* playgroundRender = physicsPlaygroundObj.AddComponent<ModelInstance>();
		playgroundRender->Init(playgroundModel.get());
		physicsPlaygroundObj.SetScale(glm::vec3(0.15f, 0.15f, 0.15f));
		physicsPlaygroundObj.SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		physicsPlaygroundObj.SetYawRotation(0);

		StaticCollider* physicsPlaygroundCollider = physicsPlaygroundObj.AddComponent<StaticCollider>();
		physicsPlaygroundCollider->Init();
		physicsPlaygroundCollider->SetColliderToModel(*playgroundModel);

		PlayerTriggerComponent* playerTrigger = physicsPlaygroundObj.AddComponent<PlayerTriggerComponent>();
		playerTrigger->SetOnPlayerEntered([&]() {
			// LEI_TRACE("Player entered trigger collider");
		});

		// Islands
		Entity& islandObj = AddEntity("Island");
		islandObj.SetScale(glm::vec3(60.0f, 60.0f, 60.0f));
		islandObj.SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		islandObj.SetYawRotation(0);

		ModelInstance* islandRender = islandObj.AddComponent<ModelInstance>();
		islandRender->Init(m_EnviromentModels["island"].get());

		StaticCollider* islandCollider = islandObj.AddComponent<StaticCollider>();
		islandCollider->Init();
		islandCollider->SetColliderToModel(*m_EnviromentModels["island"].get());

		Entity& islandObj2 = AddEntity("Island2");
		islandObj2.SetScale(glm::vec3(60.0f, 60.0f, 60.0f));
		islandObj2.SetPosition(glm::vec3(-50.0f, 300.0f, 900.0f));
		islandObj2.SetYawRotation(45);

		ModelInstance* islandRender2 = islandObj2.AddComponent<ModelInstance>();
		islandRender2->Init(m_EnviromentModels["island"].get());

		StaticCollider* islandCollider2 = islandObj2.AddComponent<StaticCollider>();
		islandCollider2->Init();
		islandCollider2->SetColliderToModel(*m_EnviromentModels["island"].get());

		Entity& islandObj3 = AddEntity("Island3");
		islandObj3.SetScale(glm::vec3(60.0f, 60.0f, 60.0f));
		islandObj3.SetPosition(glm::vec3(-700.0f, 100.0f, 200.0f));
		islandObj3.SetYawRotation(-45);

		ModelInstance* islandRender3 = islandObj3.AddComponent<ModelInstance>();
		islandRender3->Init(m_EnviromentModels["island"].get());

		StaticCollider* islandCollider3 = islandObj3.AddComponent<StaticCollider>();
		islandCollider3->Init();
		islandCollider3->SetColliderToModel(*m_EnviromentModels["island"].get());

		Entity& islandObj4 = AddEntity("Island4");
		islandObj4.SetScale(glm::vec3(60.0f, 60.0f, 60.0f));
		islandObj4.SetPosition(glm::vec3(700.0f, 100.0f, 200.0f));
		islandObj4.SetYawRotation(90);

		ModelInstance* islandRender4 = islandObj4.AddComponent<ModelInstance>();
		islandRender4->Init(m_EnviromentModels["island"].get());

		StaticCollider* islandCollider4 = islandObj4.AddComponent<StaticCollider>();
		islandCollider4->Init();
		islandCollider4->SetColliderToModel(*m_EnviromentModels["island"].get());

		Entity& islandObj5 = AddEntity("Island5");
		islandObj5.SetScale(glm::vec3(60.0f, 60.0f, 60.0f));
		islandObj5.SetPosition(glm::vec3(400.0f, -100.0f, -600.0f));
		islandObj5.SetYawRotation(-90);

		ModelInstance* islandRender5 = islandObj5.AddComponent<ModelInstance>();
		islandRender5->Init(m_EnviromentModels["island"].get());

		StaticCollider* islandCollider5 = islandObj5.AddComponent<StaticCollider>();
		islandCollider5->Init();
		islandCollider5->SetColliderToModel(*m_EnviromentModels["island"].get());

		Entity& islandObj6 = AddEntity("Island6");
		islandObj6.SetScale(glm::vec3(60.0f, 60.0f, 60.0f));
		islandObj6.SetPosition(glm::vec3(-400.0f, -100.0f, -600.0f));
		islandObj6.SetYawRotation(135);

		ModelInstance* islandRender6 = islandObj6.AddComponent<ModelInstance>();
		islandRender6->Init(m_EnviromentModels["island"].get());

		StaticCollider* islandCollider6 = islandObj6.AddComponent<StaticCollider>();
		islandCollider6->Init();
		islandCollider6->SetColliderToModel(*m_EnviromentModels["island"].get());

		// House
		Entity& houseObj = AddEntity("House");
		houseObj.SetScale(glm::vec3(4.0f, 6.0f, 4.0f));
		houseObj.SetPosition(glm::vec3(-90.0f, 300.0f, 930.0f));
		houseObj.SetYawRotation(0);

		ModelInstance* houseRender = houseObj.AddComponent<ModelInstance>();
		houseRender->Init(m_EnviromentModels["house"].get());

		StaticCollider* houseCollider = houseObj.AddComponent<StaticCollider>();
		houseCollider->Init();
		houseCollider->SetColliderToModel(*m_EnviromentModels["house"].get());

		// Fish
		Entity& fishObj = AddEntity("Fish");
		fishObj.SetScale(glm::vec3(25.0f, 25.0f, 25.0f));
		fishObj.SetPosition(glm::vec3(50.0f, 300.0f, 900.0f));
		fishObj.SetYawRotation(0);

		ModelInstance* fishRender = fishObj.AddComponent<ModelInstance>();
		fishRender->Init(m_EnviromentModels["fish"].get());

		Entity& fishObj2 = AddEntity("Fish2");
		fishObj2.SetScale(glm::vec3(25.0f, 25.0f, 25.0f));
		fishObj2.SetPosition(glm::vec3(-500.0f, 100.0f, 700.0f));
		fishObj2.SetYawRotation(45);

		ModelInstance* fishRender2 = fishObj2.AddComponent<ModelInstance>();
		fishRender2->Init(m_EnviromentModels["fish"].get());

		Entity& fishObj3 = AddEntity("Fish3");
		fishObj3.SetScale(glm::vec3(25.0f, 25.0f, 25.0f));
		fishObj3.SetPosition(glm::vec3(500.0f, 100.0f, 700.0f));
		fishObj3.SetYawRotation(-45);

		ModelInstance* fishRender3 = fishObj3.AddComponent<ModelInstance>();
		fishRender3->Init(m_EnviromentModels["fish"].get());

		Entity& fishObj4 = AddEntity("Fish4");
		fishObj4.SetScale(glm::vec3(25.0f, 25.0f, 25.0f));
		fishObj4.SetPosition(glm::vec3(600.0f, 0.0f, -200.0f));
		fishObj4.SetYawRotation(45);

		ModelInstance* fishRender4 = fishObj4.AddComponent<ModelInstance>();
		fishRender4->Init(m_EnviromentModels["fish"].get());

		Entity& fishObj5 = AddEntity("Fish5");
		fishObj5.SetScale(glm::vec3(25.0f, 25.0f, 25.0f));
		fishObj5.SetPosition(glm::vec3(-600.0f, 0.0f, -200.0f));
		fishObj5.SetYawRotation(-45);

		ModelInstance* fishRender5 = fishObj5.AddComponent<ModelInstance>();
		fishRender5->Init(m_EnviromentModels["fish"].get());

		// Trees
		Entity& treeObj = AddEntity("Tree");
		treeObj.SetScale(glm::vec3(7.0f, 7.0f, 7.0f));
		treeObj.SetPosition(glm::vec3(20.0f, 0.0f, -20.0f));
		treeObj.SetYawRotation(0);

		ModelInstance* treeRender = treeObj.AddComponent<ModelInstance>();
		treeRender->Init(m_EnviromentModels["tree"].get());

		StaticCollider* treeCollider = treeObj.AddComponent<StaticCollider>();
		treeCollider->Init();
		treeCollider->SetColliderToModel(*m_EnviromentModels["tree"].get());

		Entity& treeObj2 = AddEntity("Tree2");
		treeObj2.SetScale(glm::vec3(7.0f, 7.0f, 7.0f));
		treeObj2.SetPosition(glm::vec3(-720.0f, 100.0f, 220.0f));
		treeObj2.SetYawRotation(45);

		ModelInstance* treeRender2 = treeObj2.AddComponent<ModelInstance>();
		treeRender2->Init(m_EnviromentModels["tree"].get());

		StaticCollider* treeCollider2 = treeObj2.AddComponent<StaticCollider>();
		treeCollider2->Init();
		treeCollider2->SetColliderToModel(*m_EnviromentModels["tree"].get());

		Entity& treeObj3 = AddEntity("Tree3");
		treeObj3.SetScale(glm::vec3(7.0f, 7.0f, 7.0f));
		treeObj3.SetPosition(glm::vec3(710.0f, 100.0f, 170.0f));
		treeObj3.SetYawRotation(-45);

		ModelInstance* treeRender3 = treeObj3.AddComponent<ModelInstance>();
		treeRender3->Init(m_EnviromentModels["tree"].get());

		StaticCollider* treeCollider3 = treeObj3.AddComponent<StaticCollider>();
		treeCollider3->Init();
		treeCollider3->SetColliderToModel(*m_EnviromentModels["tree"].get());

		Entity& treeObj4 = AddEntity("Tree4");
		treeObj4.SetScale(glm::vec3(7.0f, 7.0f, 7.0f));
		treeObj4.SetPosition(glm::vec3(380.0f, -100.0f, -620.0f));
		treeObj4.SetYawRotation(90);

		ModelInstance* treeRender4 = treeObj4.AddComponent<ModelInstance>();
		treeRender4->Init(m_EnviromentModels["tree"].get());

		StaticCollider* treeCollider4 = treeObj4.AddComponent<StaticCollider>();
		treeCollider4->Init();
		treeCollider4->SetColliderToModel(*m_EnviromentModels["tree"].get());

		Entity& treeObj5 = AddEntity("Tree5");
		treeObj5.SetScale(glm::vec3(7.0f, 7.0f, 7.0f));
		treeObj5.SetPosition(glm::vec3(-420.0f, -100.0f, -620.0f));
		treeObj5.SetYawRotation(-90);

		ModelInstance* treeRender5 = treeObj5.AddComponent<ModelInstance>();
		treeRender5->Init(m_EnviromentModels["tree"].get());

		StaticCollider* treeCollider5 = treeObj5.AddComponent<StaticCollider>();
		treeCollider5->Init();
		treeCollider5->SetColliderToModel(*m_EnviromentModels["tree"].get());

		Entity& treeObj6 = AddEntity("Tree6");
		treeObj6.SetScale(glm::vec3(6.0f, 6.0f, 6.0f));
		treeObj6.SetPosition(glm::vec3(10.0f, 0.0f, 20.0f));
		treeObj6.SetYawRotation(60);

		ModelInstance* treeRender6 = treeObj6.AddComponent<ModelInstance>();
		treeRender6->Init(m_EnviromentModels["tree"].get());

		StaticCollider* treeCollider6 = treeObj6.AddComponent<StaticCollider>();
		treeCollider6->Init();
		treeCollider6->SetColliderToModel(*m_EnviromentModels["tree"].get());

		Entity& treeObj7 = AddEntity("Tree7");
		treeObj7.SetScale(glm::vec3(8.0f, 8.0f, 8.0f));
		treeObj7.SetPosition(glm::vec3(-40.0f, 0.0f, 0.0f));
		treeObj7.SetYawRotation(-40);

		ModelInstance* treeRender7 = treeObj7.AddComponent<ModelInstance>();
		treeRender7->Init(m_EnviromentModels["tree"].get());

		StaticCollider* treeCollider7 = treeObj7.AddComponent<StaticCollider>();
		treeCollider7->Init();
		treeCollider7->SetColliderToModel(*m_EnviromentModels["tree"].get());

		Entity& treeObj8 = AddEntity("Tree8");
		treeObj8.SetScale(glm::vec3(5.0f, 5.0f, 5.0f));
		treeObj8.SetPosition(glm::vec3(-690.0f, 100.0f, 170.0f));
		treeObj8.SetYawRotation(-80);

		ModelInstance* treeRender8 = treeObj8.AddComponent<ModelInstance>();
		treeRender8->Init(m_EnviromentModels["tree"].get());

		StaticCollider* treeCollider8 = treeObj8.AddComponent<StaticCollider>();
		treeCollider8->Init();
		treeCollider8->SetColliderToModel(*m_EnviromentModels["tree"].get());

		Entity& treeObj9 = AddEntity("Tree9");
		treeObj9.SetScale(glm::vec3(9.0f, 9.0f, 9.0f));
		treeObj9.SetPosition(glm::vec3(680.0f, 100.0f, 220.0f));
		treeObj9.SetYawRotation(115);

		ModelInstance* treeRender9 = treeObj9.AddComponent<ModelInstance>();
		treeRender9->Init(m_EnviromentModels["tree"].get());

		StaticCollider* treeCollider9 = treeObj9.AddComponent<StaticCollider>();
		treeCollider9->Init();
		treeCollider9->SetColliderToModel(*m_EnviromentModels["tree"].get());

		Entity& treeObj10 = AddEntity("Tree10");
		treeObj10.SetScale(glm::vec3(8.0f, 8.0f, 8.0f));
		treeObj10.SetPosition(glm::vec3(410.0f, -100.0f, -580.0f));
		treeObj10.SetYawRotation(10);

		ModelInstance* treeRender10 = treeObj10.AddComponent<ModelInstance>();
		treeRender10->Init(m_EnviromentModels["tree"].get());

		StaticCollider* treeCollider10 = treeObj10.AddComponent<StaticCollider>();
		treeCollider10->Init();
		treeCollider10->SetColliderToModel(*m_EnviromentModels["tree"].get());

		Entity& treeObj11 = AddEntity("Tree11");
		treeObj11.SetScale(glm::vec3(6.0f, 6.0f, 6.0f));
		treeObj11.SetPosition(glm::vec3(420.0f, -100.0f, -610.0f));
		treeObj11.SetYawRotation(-40);

		ModelInstance* treeRender11 = treeObj11.AddComponent<ModelInstance>();
		treeRender11->Init(m_EnviromentModels["tree"].get());

		StaticCollider* treeCollider11 = treeObj11.AddComponent<StaticCollider>();
		treeCollider11->Init();
		treeCollider11->SetColliderToModel(*m_EnviromentModels["tree"].get());

		Entity& treeObj12 = AddEntity("Tree12");
		treeObj12.SetScale(glm::vec3(8.0f, 8.0f, 8.0f));
		treeObj12.SetPosition(glm::vec3(-390.0f, -100.0f, -580.0f));
		treeObj12.SetYawRotation(-20);

		ModelInstance* treeRender12 = treeObj12.AddComponent<ModelInstance>();
		treeRender12->Init(m_EnviromentModels["tree"].get());

		StaticCollider* treeCollider12 = treeObj12.AddComponent<StaticCollider>();
		treeCollider12->Init();
		treeCollider12->SetColliderToModel(*m_EnviromentModels["tree"].get());

		Entity& treeObj13 = AddEntity("Tree13");
		treeObj13.SetScale(glm::vec3(6.0f, 6.0f, 6.0f));
		treeObj13.SetPosition(glm::vec3(-380.0f, -100.0f, -610.0f));
		treeObj13.SetYawRotation(60);

		ModelInstance* treeRender13 = treeObj13.AddComponent<ModelInstance>();
		treeRender13->Init(m_EnviromentModels["tree"].get());

		StaticCollider* treeCollider13 = treeObj13.AddComponent<StaticCollider>();
		treeCollider13->Init();
		treeCollider13->SetColliderToModel(*m_EnviromentModels["tree"].get());

		// Test color source
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

		// Flowers ---------------------
		Entity& flowerObj1 = AddEntity("Flower 1");
		flowerObj1.SetYawRotation(0);
		ModelInstance* flowerRender = flowerObj1.AddComponent<ModelInstance>();
		flowerRender->Init(m_EnviromentModels["flower"].get());
		flowerObj1.SetScale(glm::vec3(2.5f, 2.5f, 2.5f));
		flowerObj1.SetPosition(glm::vec3(-90.0f, 178.3f, 446.0f));
		ColorSource* startSrc = flowerObj1.AddComponent<ColorSource>();
		startSrc->Init(5, 10, true);
		StaticCollider* flowerCollider1 = flowerObj1.AddComponent<StaticCollider>();
		flowerCollider1->Init();
		flowerCollider1->SetColliderToModel(*m_EnviromentModels["flower"].get(), true);
		PlayerTriggerComponent* playerTrigger1 = flowerObj1.AddComponent<PlayerTriggerComponent>();
		playerTrigger1->SetOnPlayerEntered([&]() {
			if (!color_1)
			{
				color_1 = true;
				// this is a code smell, but I cannot think of a cleaner way to do this
				dynamic_cast<BaseGuiScreen&>(GuiManager::Instance().GetBaseScreen()).AddFlower();
				AudioPlayer::GetInstance().PlaySFX("collect.mp3");
				flowerObj1.SetScale(glm::vec3(0.0f, 0.0f, 0.0f));
			}
		});

		Entity& flowerObj2 = AddEntity("Flower 2");
		flowerObj2.SetYawRotation(60);
		ModelInstance* flowerRender2 = flowerObj2.AddComponent<ModelInstance>();
		flowerRender2->Init(m_EnviromentModels["flower"].get());
		flowerObj2.SetScale(glm::vec3(2.4f, 2.4f, 2.4f));
		flowerObj2.SetPosition(glm::vec3(-91.5f, -233.6f, -467.0f));
		ColorSource* startSrc2 = flowerObj2.AddComponent<ColorSource>();
		startSrc2->Init(5, 10, true);
		StaticCollider* flowerCollider2 = flowerObj2.AddComponent<StaticCollider>();
		flowerCollider2->Init();
		flowerCollider2->SetColliderToModel(*m_EnviromentModels["flower"].get(), true);
		PlayerTriggerComponent* playerTrigger2 = flowerObj2.AddComponent<PlayerTriggerComponent>();
		playerTrigger2->SetOnPlayerEntered([&]() {
			if (!color_2)
			{
				color_2 = true;
				// this is a code smell, but I cannot think of a cleaner way to do this
				dynamic_cast<BaseGuiScreen&>(GuiManager::Instance().GetBaseScreen()).AddFlower();
				AudioPlayer::GetInstance().PlaySFX("collect.mp3");
				flowerObj2.SetScale(glm::vec3(0.0f, 0.0f, 0.0f));
			}
		});

		Entity& flowerObj3 = AddEntity("Flower 3");
		flowerObj3.SetYawRotation(0);
		ModelInstance* flowerRender3 = flowerObj3.AddComponent<ModelInstance>();
		flowerRender3->Init(m_EnviromentModels["flower"].get());
		flowerObj3.SetScale(glm::vec3(2.5f, 2.5f, 2.5f));
		flowerObj3.SetPosition(glm::vec3(-89.5f, -233.6f, -467.0f));
		ColorSource* startSrc3 = flowerObj3.AddComponent<ColorSource>();
		startSrc3->Init(5, 10, true);
		StaticCollider* flowerCollider3 = flowerObj3.AddComponent<StaticCollider>();
		flowerCollider3->Init();
		flowerCollider3->SetColliderToModel(*m_EnviromentModels["flower"].get(), true);
		PlayerTriggerComponent* playerTrigger3 = flowerObj3.AddComponent<PlayerTriggerComponent>();
		playerTrigger3->SetOnPlayerEntered([&]() {
			if (!color_3)
			{
				color_3 = true;
				// this is a code smell, but I cannot think of a cleaner way to do this
				dynamic_cast<BaseGuiScreen&>(GuiManager::Instance().GetBaseScreen()).AddFlower();
				AudioPlayer::GetInstance().PlaySFX("collect.mp3");
				flowerObj3.SetScale(glm::vec3(0.0f, 0.0f, 0.0f));
			}
		});

		////Test Multiple Components
		Entity& skyboxObj = AddEntity("Skybox");

		SkyBox* skybox = skyboxObj.AddComponent<SkyBox>();
		std::vector<std::string> faces{ "data/skybox/jessica_sky/right.jpg", "data/skybox/jessica_sky/left.jpg",
			"data/skybox/jessica_sky/py.jpg", "data/skybox/jessica_sky/down.jpg",
			"data/skybox/jessica_sky/front.jpg", "data/skybox/jessica_sky/back.jpg" };
		skybox->Init(faces);

		AudioPlayer::GetInstance().PlayMusic("sus2.mp3", 0.3f);
		GuiManager::Instance().QueueNextScreen(nullptr);
	}

	void TestSceneKevin::OnReset()
	{
		// Just need to reset the backpack.
		Entity* backpackObj = GetEntity("Backpack");
		backpackObj->SetScale(glm::vec3(1.f, 1.f, 1.f));
		backpackObj->SetPosition(glm::vec3(-112.5f, 505.f, 3.f));
		AudioPlayer::GetInstance().PlaySFX("reset.mp3");
	}

	void TestSceneKevin::OnUpdate()
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
			colorObj4->GetComponent<ColorSource>()->SetActive(true);
			colorObj4->GetComponent<ColorSource>()->radius += deltaColorRadius * 2;
			if (!m_Won)
			{
				AudioPlayer::GetInstance().StopMusic("sus2.mp3");
				AudioPlayer::GetInstance().PlayMusic("MUS-END_CREDITS.wav", 0.3);
				m_Won = true;
			}
		}
	}

	void TestSceneKevin::OnPhysicsUpdate()
	{
		// std::cout << "OnPhysicsUpdate frame tick" << (Application::DeltaTime()) << std::endl;
		m_PhysicsWorld->Step(Application::DeltaTime());
	}
} // namespace lei3d
