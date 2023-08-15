#include "TestSceneKevin.hpp"

#include "components/ModelRenderer.hpp"

#include "logging/GLDebug.hpp"

#include <glm/glm.hpp>
#include "physics/PhysicsWorld.hpp"



namespace lei3d {
    TestSceneKevin::TestSceneKevin() {
    }

    TestSceneKevin::~TestSceneKevin() {

    }

    void TestSceneKevin::OnLoad() {

        // load textures
        stbi_set_flip_vertically_on_load(true);

        // Load Models
        const std::string backpackPath = "data/models/backpack/backpack.obj";
        backpackModel = std::make_unique<Model>(backpackPath);
        const std::string physicsPlaygroundPath = "data/models/leveldesign/KevWorldColorFive.obj";
        playgroundModel = std::make_unique<Model>(physicsPlaygroundPath);

        //BACKPACK (Character) ---------------------
        Entity& backpackObj = AddEntity("Backpack");

        ModelRenderer* modelRender = backpackObj.AddComponent<ModelRenderer>();
        modelRender->Init(backpackModel.get(), m_MainShader.get());
        backpackObj.SetScale(glm::vec3(1.f, 1.f, 1.f));
        backpackObj.SetPosition(glm::vec3(0.f, 200.f, 0.f));

        CharacterController* characterController = backpackObj.AddComponent<CharacterController>();
        characterController->Init();

        //PHYSICS PLAYGROUND---------------------
        Entity& physicsPlaygroundObj = AddEntity("Physics Playground");

        ModelRenderer* playgroundRender = physicsPlaygroundObj.AddComponent<ModelRenderer>();
        playgroundRender->Init(playgroundModel.get(), m_MainShader.get());
        physicsPlaygroundObj.SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
        physicsPlaygroundObj.SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));

        StaticCollider* physicsPlaygroundCollider = physicsPlaygroundObj.AddComponent<StaticCollider>();
        physicsPlaygroundCollider->Init();
        physicsPlaygroundCollider->SetColliderToModel(*playgroundModel);

        ////Test Multiple Components
        Entity& skyboxObj = AddEntity("Skybox");

        SkyBox* skybox = skyboxObj.AddComponent<SkyBox>();
        std::vector<std::string> faces
        {
            "data/skybox/anime_etheria/right.jpg",
            "data/skybox/anime_etheria/left.jpg",
            "data/skybox/anime_etheria/up.jpg",
            "data/skybox/anime_etheria/down.jpg",
            "data/skybox/anime_etheria/front.jpg",
            "data/skybox/anime_etheria/back.jpg"
        };
        skybox->Init(faces);

        // THIS IS TEMPORARY AND FOR TESTING
        m_AudioPlayer = std::make_unique<AudioPlayer>(); 
    }

    void TestSceneKevin::OnUpdate(float deltaTime) {
        GLFWwindow* const window = Application::Curr()->Window();
        

        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        {
            Entity* backpackEntity = GetEntity("Backpack");
            if (backpackEntity)
            {
                std::cout << "backpackEntity" << std::endl;
                backpackEntity->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f)); // IS THIS OKAY? DOES IT CAUSE A MEMORY LEAK?
            }
        }
	
    }

    void TestSceneKevin::OnPhysicsUpdate(float deltaTime) {
        m_PhysicsWorld->Step(deltaTime);
    }

    void TestSceneKevin::OnRender() {}
}