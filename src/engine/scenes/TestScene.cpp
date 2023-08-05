#include "TestScene.hpp"

#include "components/ModelRenderer.hpp"

#include "logging/GLDebug.hpp"

#include <glm/glm.hpp>
#include "physics/PhysicsWorld.hpp"



namespace lei3d {
    TestScene::TestScene() {
    }

    TestScene::~TestScene() {

    }

    void TestScene::LoadObjects() {
        // load shaders
        m_MainShader = std::make_unique<Shader>("./data/shaders/transformations.vert", "./data/shaders/transformations.frag");
        m_PhysicsWorld = std::make_unique<PhysicsWorld>();
        m_PhysicsWorld->Create();    //TODO: Consider if there is some better way to do this

        // load textures
        stbi_set_flip_vertically_on_load(true);

        // Load Models
        const std::string backpackPath = "data/models/backpack/backpack.obj";
        backpackModel = std::make_unique<Model>(backpackPath);
        const std::string physicsPlaygroundPath = "data/models/leveldesign/KekkekinPlayground.obj";
        playgroundModel = std::make_unique<Model>(physicsPlaygroundPath);

        //BACKPACK (Character) ---------------------
        std::unique_ptr<Entity> backpackObj = std::make_unique<Entity>();

        ModelRenderer* modelRender = backpackObj->AddComponent<ModelRenderer>();
        modelRender->Init(backpackModel.get(), m_MainShader.get());
        backpackObj->SetScale(glm::vec3(1.f, 1.f, 1.f));
        backpackObj->SetPosition(glm::vec3(0.f, 200.f, 0.f));

        CharacterController* characterController = backpackObj->AddComponent<CharacterController>();
        characterController->Init();

        m_Entities.push_back(std::move(backpackObj));

        //PHYSICS PLAYGROUND---------------------
        std::unique_ptr<Entity> physicsPlaygroundObj = std::make_unique<Entity>();

        ModelRenderer* playgroundRender = physicsPlaygroundObj->AddComponent<ModelRenderer>();
        playgroundRender->Init(playgroundModel.get(), m_MainShader.get());
        physicsPlaygroundObj->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
        physicsPlaygroundObj->SetPosition(glm::vec3(0.0f, -10.f, 0.f));

        StaticCollider* physicsPlaygroundCollider = physicsPlaygroundObj->AddComponent<StaticCollider>();
        physicsPlaygroundCollider->Init();
        physicsPlaygroundCollider->SetColliderToModel(*playgroundModel);

        m_Entities.push_back(std::move(physicsPlaygroundObj));

        //Test Multiple Components
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

    void TestScene::OnUpdate(float deltaTime) {

    }

    void TestScene::OnPhysicsUpdate(float deltaTime) {
        m_PhysicsWorld->Step(deltaTime);
    }

    void TestScene::OnRender() {
        //FOR TESTING, NO LONGER BEING USED IN-GAME

        //LEI_TRACE("Rendering Test Scene");
        // rendering
        //GLCall(glClearColor(0.2f, 0.8f, 0.9f, 1.0f));
        //GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        // Use transformation Shader for Rendering Main Object
        //m_MainShader.use();

        //// -- Set up camera views and pass to shader
        //glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 400.0f);
        //m_MainShader->setUniformMat4(projection, "proj");

        //glm::mat4 view = m_Camera->getCameraView();
        //m_MainShader->setUniformMat4(view, "view");


        //// RENDER FIRST OBJECT
        //m_Backpack.SetScale(glm::vec3(0.25, 0.25, 0.25));
        //glm::mat4 model = glm::mat4(1.0f);
        //model = glm::scale(model, m_Backpack.transform.scale);

        //// -- translate the backpack
        //glm::vec3 physicsPos = GetFirstColliderPosition(m_PhysicsObjects);
        //m_Backpack.SetPosition(physicsPos);
        //model = glm::translate(model, m_Backpack.transform.position);
        //m_MainShader.setUniformMat4(model, "model");

        //// -- draw mesh
        //m_Backpack.m_Model->Draw(m_MainShader);

        ////RENDER SECOND OBJECT

        //model = glm::mat4(1.0f);
        //m_Backpack.SetScale(glm::vec3(5.25, 3.25, 2.25));
        //model = glm::scale(model, m_Backpack.transform.scale);
        //m_MainShader.setUniformMat4(model, "model");

        //// -- draw mesh
        //m_Backpack.m_Model->Draw(m_MainShader);

        //// RENDER THIRD OBJECT

        //model = glm::mat4(1.0f);
        //m_Backpack.SetScale(glm::vec3(1.25, 1.25, 1.25));
        //model = glm::scale(model, m_Backpack.transform.scale);
        //m_MainShader.setUniformMat4(model, "model");

        //// -- draw mesh
        //m_Backpack.m_Model->Draw(m_MainShader);


        // render skybox after rendering rest of the scene (only draw skybox where an object is not present)
        //GLCall(glDepthFunc(GL_LEQUAL)); // we change the depth function here to it passes when testingdepth value is equal to what is current stored
        //m_Skybox.skyboxShader.use();
        //glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 400.0f);
        //m_Skybox.skyboxShader.setUniformMat4(projection, "proj");
        //glm::mat4 view = glm::mat4(glm::mat3(m_Camera->GetCameraVP()));
        //m_Skybox->skyboxShader.bind();
        //glm::mat4 skyboxMVP = m_Camera->GetProj() * glm::mat4(glm::mat3(m_Camera->GetView()));
        //m_Skybox.skyboxShader.setUniformMat4("u_MVP", skyboxMVP);

        //// -- render the skybox cube
        //m_Skybox.Render();
    }
}