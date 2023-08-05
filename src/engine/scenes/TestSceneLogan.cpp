#include "TestSceneLogan.hpp"

#include "components/ModelRenderer.hpp"

#include "logging/GLDebug.hpp"

#include <glm/glm.hpp>
#include "physics/PhysicsWorld.hpp"



namespace lei3d {
    TestSceneLogan::TestSceneLogan() {
    }

    TestSceneLogan::~TestSceneLogan() {

    }

    void TestSceneLogan::LoadObjects() {

        // load textures
        stbi_set_flip_vertically_on_load(true);

        // NOTE: We haven't implemented transform changing component stuff yet so the backpack might render weird/be really big if you uncomment.
        const std::string modelPath = "data/models/backpack/backpack.obj";

        // Load Models
        const std::string backpackPath = "data/models/backpack/backpack.obj";
        backpackModel = std::make_unique<Model>(backpackPath);

        //BACKPACK ---------------------
        std::unique_ptr<Entity> backpackObj = std::make_unique<Entity>();

        ModelRenderer* modelRender = backpackObj->AddComponent<ModelRenderer>();
        modelRender->Init(backpackModel.get(), m_MainShader.get());
        backpackObj->SetScale(glm::vec3(1.25f, 1.25f, 1.25f));
        backpackObj->SetPosition(glm::vec3(0.f, 0.f, 0.f));

        m_Entities.push_back(std::move(backpackObj));

        //BACKPACK 2 ---------------------
        std::unique_ptr<Entity> backpackObj2 = std::make_unique<Entity>();
         
        ModelRenderer* modelRender2 = backpackObj2->AddComponent<ModelRenderer>();
        modelRender2->Init(backpackModel.get(), m_MainShader.get());
        backpackObj2->SetScale(glm::vec3(0.25f, 0.25f, 0.25f));
        backpackObj2->SetPosition(glm::vec3(10.f, 0.f, 0.f));
         
        m_Entities.push_back(std::move(backpackObj2));

        ////Test Multiple Components
        std::unique_ptr<Entity> skyboxObj = std::make_unique<Entity>();
        SkyBox* skybox = skyboxObj->AddComponent<SkyBox>();
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
        m_Entities.push_back(std::move(skyboxObj));
    }

    void TestSceneLogan::OnUpdate(float deltaTime) {

    }

    void TestSceneLogan::OnPhysicsUpdate(float deltaTime) {
        m_PhysicsWorld->Step(deltaTime);
    }

    void TestSceneLogan::OnRender() {
    }
}