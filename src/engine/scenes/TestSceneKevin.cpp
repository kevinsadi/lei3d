#include "TestSceneKevin.hpp"

#include "core/Application.hpp"

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

		const std::string physicsPlaygroundPath = "data/models/skyramps/skyramps.obj";
		if (playgroundModel)
		{
			playgroundModel.reset();
		}
		playgroundModel = std::make_unique<Model>(physicsPlaygroundPath);

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

		// Timer Component EXAMPLE ----------
		TimerComponent* timerComponent = backpackObj.AddComponent<TimerComponent>();
		timerComponent->SetTargetTime(5.0f);
		timerComponent->OnTimerEnd([&]() {
			LEI_TRACE("Timer Ended!!!");
		});
		timerComponent->StartTimer();
		// -----------------------------------

		// PHYSICS PLAYGROUND---------------------
		Entity& physicsPlaygroundObj = AddEntity("Physics Playground");

		ModelInstance* playgroundRender = physicsPlaygroundObj.AddComponent<ModelInstance>();
		playgroundRender->Init(playgroundModel.get());
		physicsPlaygroundObj.SetScale(glm::vec3(0.2f, 0.2f, 0.2f));
		physicsPlaygroundObj.SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		physicsPlaygroundObj.SetYawRotation(0);

		StaticCollider* physicsPlaygroundCollider = physicsPlaygroundObj.AddComponent<StaticCollider>();
		physicsPlaygroundCollider->Init();
		physicsPlaygroundCollider->SetColliderToModel(*playgroundModel);

		PlayerTriggerComponent* playerTrigger = physicsPlaygroundObj.AddComponent<PlayerTriggerComponent>();
		playerTrigger->SetOnPlayerEntered([&]() {
			LEI_TRACE("Player entered trigger collider");
		});

		// Test color source
		Entity& startColorSrcObj = AddEntity("Start Color Area");
		ColorSource* startSrc = startColorSrcObj.AddComponent<ColorSource>();
		startSrc->Init(200, 10, true);
		startColorSrcObj.SetPosition(glm::vec3(-112.5, 505, 3));

		////Test Multiple Components
		Entity& skyboxObj = AddEntity("Skybox");

		SkyBox* skybox = skyboxObj.AddComponent<SkyBox>();
		std::vector<std::string> faces{ "data/skybox/anime_etheria/right.jpg", "data/skybox/anime_etheria/left.jpg",
			"data/skybox/anime_etheria/up.jpg", "data/skybox/anime_etheria/down.jpg",
			"data/skybox/anime_etheria/front.jpg", "data/skybox/anime_etheria/back.jpg" };
		skybox->Init(faces);

		AudioPlayer::GetInstance().PlayMusic("sus2.mp3", 0.1f);
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
		// if (glfwGetKey(Application::Window(), GLFW_KEY_R) == GLFW_PRESS)
		//{
		//	Entity* backpackEntity = GetEntity("Backpack");
		//	if (backpackEntity)
		//	{
		//		std::cout << "backpackEntity" << std::endl;
		//		backpackEntity->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		//	}
		// }
	}

	void TestSceneKevin::OnPhysicsUpdate()
	{
		m_PhysicsWorld->Step(Application::DeltaTime());
	}
} // namespace lei3d
