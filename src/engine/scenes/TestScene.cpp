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

    void TestScene::LoadObjects() {
        // load shaders
        m_MainShader = std::make_shared<Shader>("./data/shaders/transformations.vert", "./data/shaders/transformations.frag");

        // load textures
        stbi_set_flip_vertically_on_load(true);

        // load mesh from obj file (EVENTUALLY WILL WANT TO USE GTLF FILES INSTEAD)
        // NOTE: We haven't implemented transform changing component stuff yet so the backpack might render weird/be really big if you uncomment.
        //const std::string modelPath = "data/models/backpack/backpack.obj";
        //std::shared_ptr<Entity> backpackObj = std::make_shared<Entity>();
        //backpackObj->AddComponent<Backpack>();
        //std::shared_ptr<Model> model = backpackObj->AddComponent<Model>();
        //model->Init(modelPath, *m_MainShader);
        //m_Entities.push_back(backpackObj);

        std::shared_ptr<Entity> skyboxObj = std::make_shared<Entity>();
        std::shared_ptr<SkyBox> skybox = skyboxObj->AddComponent<SkyBox>();
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
        m_Entities.push_back(skyboxObj);
    }

    void TestScene::OnUpdate(float deltaTime) {

    }

    void TestScene::OnRender() {
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