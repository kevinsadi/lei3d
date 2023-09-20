#include "TestSceneAvery.hpp"

#include "core/Application.hpp"

#include "components/ModelInstance.hpp"
#include "components/CharacterController.hpp"
#include "components/SkyBox.hpp"
#include "components/StaticCollider.hpp"
#include "components/FollowCameraController.hpp"

#include "logging/GLDebug.hpp"
#include "physics/PhysicsWorld.hpp"

#include "audio/AudioPlayer.hpp"

#include <glm/glm.hpp>

namespace lei3d
{
	std::unique_ptr<Scene> MakeTestSceneAvery()
	{
		return std::make_unique<TestSceneAvery>();
	}

	TestSceneAvery::TestSceneAvery()
	{
	}

	TestSceneAvery::~TestSceneAvery()
	{
	}

	void TestSceneAvery::OnLoad()
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
		const std::string physicsPlaygroundPath = "data/models/leveldesign/KevWorldClouds.obj";
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
		backpackObj.SetPosition(glm::vec3(0.f, 200.f, 0.f));
		backpackObj.SetYawRotation(0);

		CharacterController* characterController = backpackObj.AddComponent<CharacterController>();
		characterController->Init(1.f, 3.f);

		FollowCameraController* followCam = backpackObj.AddComponent<FollowCameraController>();
		followCam->Init(*m_DefaultCamera, glm::vec3(0.0f, 1.0f, 0.0f));

		// PHYSICS PLAYGROUND---------------------
		Entity& physicsPlaygroundObj = AddEntity("Physics Playground");

		ModelInstance* playgroundRender = physicsPlaygroundObj.AddComponent<ModelInstance>();
		playgroundRender->Init(playgroundModel.get());
		physicsPlaygroundObj.SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
		physicsPlaygroundObj.SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		physicsPlaygroundObj.SetYawRotation(0);

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

	void TestSceneAvery::OnUpdate()
	{
		if (glfwGetKey(Application::Window(), GLFW_KEY_R) == GLFW_PRESS)
		{
			Entity* backpackEntity = GetEntity("Backpack");
			if (backpackEntity)
			{
				std::cout << "backpackEntity" << std::endl;
				backpackEntity->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f)); // IS THIS OKAY? DOES IT CAUSE A MEMORY LEAK?
			}
		}

		if (glfwGetKey(Application::Window(), GLFW_KEY_J) == GLFW_PRESS)
		{
			std::cout << "Pressing J Key" << std::endl;
			AudioPlayer::PlaySFX("landing");
		}

		if (glfwGetKey(Application::Window(), GLFW_KEY_K) == GLFW_PRESS)
		{
			std::cout << "Pressing K Key" << std::endl;
			AudioPlayer::PlaySFXForSeconds("landing", 5, 1.0f, 2, 1);
		}

		if (glfwGetKey(Application::Window(), GLFW_KEY_L) == GLFW_PRESS)
		{
			std::cout << "Pressing L Bozo Key" << std::endl;
			AudioPlayer::PlaySFXForSeconds("../breakcore", 5, 1.0f, 1, 2);
		}
	}

	void TestSceneAvery::OnPhysicsUpdate()
	{
		m_PhysicsWorld->Step(Application::DeltaTime());
	}
} // namespace lei3d