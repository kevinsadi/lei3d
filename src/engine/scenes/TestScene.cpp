#include "TestScene.hpp"

#include "components/Backpack.hpp"
#include "components/Model.hpp"

#include "util/GLDebug.hpp"

#include <glm/glm.hpp>

namespace lei3d {
    TestScene::TestScene() {

    }

    TestScene::~TestScene() {

    }

    void TestScene::OnLoad() {
        // load shaders
        m_MainShader = std::make_unique<Shader>("./data/shaders/transformations.vert", "./data/shaders/transformations.frag");

        // load textures
        stbi_set_flip_vertically_on_load(true);

        // NOTE: We haven't implemented transform changing component stuff yet so the backpack might render weird/be really big if you uncomment.
        const std::string modelPath = "data/models/backpack/backpack.obj";

        std::unique_ptr<Entity> backpackObj = std::make_unique<Entity>();
        backpackObj->AddComponent<Backpack>();
        Model* model = backpackObj->AddComponent<Model>();
        model->Init(modelPath, *m_MainShader);
        backpackObj->SetScale(glm::vec3(1.25f, 1.25f, 1.25f));
        backpackObj->SetPosition(glm::vec3(0.f, 0.f, 0.f));
        m_Entities.push_back(std::move(backpackObj));

        std::unique_ptr<Entity> backpackObj2 = std::make_unique<Entity>();
        backpackObj2->AddComponent<Backpack>();
        Model* model2 = backpackObj2->AddComponent<Model>();
        model2->Init(modelPath, *m_MainShader);
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

        //m_PhysicsWorld.Create();    //TODO: This is temporary. Change this to use physics components.
    }

    void TestScene::OnUpdate(float deltaTime) {

    }

    void TestScene::OnPhysicsUpdate(float deltaTime) {
        //m_PhysicsWorld.Step(deltaTime);
    }
}