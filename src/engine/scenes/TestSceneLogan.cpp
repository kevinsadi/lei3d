#include "TestSceneLogan.hpp"

#include "components/ModelInstance.hpp"
#include "logging/GLDebug.hpp"
#include "physics/PhysicsWorld.hpp"

#include <glm/glm.hpp>

namespace lei3d
{

	TestSceneLogan::TestSceneLogan()
	{
	}

	TestSceneLogan::~TestSceneLogan()
	{
	}

	void TestSceneLogan::OnLoad()
	{
		// load textures
		stbi_set_flip_vertically_on_load(true);

		// NOTE: We haven't implemented transform changing component stuff yet so the backpack might render weird/be really
		// big if you uncomment.
		const std::string modelPath = "data/models/backpack/backpack.obj";

		// Load Models
		const std::string backpackPath = "data/models/backpack/backpack.obj";
		backpackModel = std::make_unique<Model>(backpackPath);

		// BACKPACK ---------------------
		Entity& backpackObj = AddEntity("Backpack");

		ModelInstance* modelRender = backpackObj.AddComponent<ModelInstance>();
		modelRender->Init(backpackModel.get());
		backpackObj.SetScale(glm::vec3(1.25f, 1.25f, 1.25f));
		backpackObj.SetPosition(glm::vec3(0.f, 0.f, 0.f));

		// BACKPACK 2 ---------------------
		Entity& backpackObj2 = AddEntity("Backpack");

		ModelInstance* modelRender2 = backpackObj2.AddComponent<ModelInstance>();
		modelRender2->Init(backpackModel.get());
		backpackObj2.SetScale(glm::vec3(0.25f, 0.25f, 0.25f));
		backpackObj2.SetPosition(glm::vec3(10.f, 0.f, 0.f));

		////Test Multiple Components
		Entity&					 skyboxObj = AddEntity("Skybox");
		SkyBox*					 skybox = skyboxObj.AddComponent<SkyBox>();
		std::vector<std::string> faces{ "data/skybox/anime_etheria/right.jpg", "data/skybox/anime_etheria/left.jpg",
			"data/skybox/anime_etheria/up.jpg", "data/skybox/anime_etheria/down.jpg",
			"data/skybox/anime_etheria/front.jpg", "data/skybox/anime_etheria/back.jpg" };
		skybox->Init(faces);

		PrintEntityList();
	}

	void TestSceneLogan::OnPhysicsUpdate()
	{
		m_PhysicsWorld->Step(Application::DeltaTime());
	}

} // namespace lei3d