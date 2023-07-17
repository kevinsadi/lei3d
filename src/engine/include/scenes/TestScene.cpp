#include "TestScene.hpp"

#include "util/GLDebug.hpp"

#include <glm/glm.hpp>

namespace lei3d {
    TestScene::TestScene() {

    }

    TestScene::~TestScene() {

    }

    void TestScene::LoadObjects() {
        // load shaders
        m_MainShader = Shader("./data/shaders/transformations.vert", "./data/shaders/transformations.frag");

        // load textures
        stbi_set_flip_vertically_on_load(true);

        // load mesh from obj file (EVENTUALLY WILL WANT TO USE GTLF FILES INSTEAD)
        std::string path = "data/models/backpack/backpack.obj";
        Model* backpackModel = new Model(path);
        m_Backpack = Entity(backpackModel);
        //std::shared_ptr<Entity> backpack = std::make_shared<Entity>(backpackModel);
        //m_Entities.push_back(backpack);

        // create skybox
        SkyBox skybox = SkyBox();

        std::vector<std::string> faces
        {
            "data/skybox/anime_etheria/right.jpg",
            "data/skybox/anime_etheria/left.jpg",
            "data/skybox/anime_etheria/up.jpg",
            "data/skybox/anime_etheria/down.jpg",
            "data/skybox/anime_etheria/front.jpg",
            "data/skybox/anime_etheria/back.jpg"
        };
        skybox.loadCubemap(faces);

        skybox.skyboxShader.use();
        skybox.skyboxShader.setInt("skybox", 0);
        m_Skybox = skybox;
    }

    void TestScene::OnUpdate() {

    }

    void TestScene::OnRender() {
        // rendering
        GLCall(glClearColor(0.2f, 0.8f, 0.9f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        // Use transformation Shader for Rendering Main Object
        m_MainShader.use();

        // -- Set up camera views and pass to shader
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 400.0f);
        m_MainShader.setUniformMat4(projection, "proj");

        glm::mat4 view = m_Camera->getCameraView();
        m_MainShader.setUniformMat4(view, "view");

        // RENDER FIRST OBJECT
        m_Backpack.SetScale(glm::vec3(0.25, 0.25, 0.25));
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, m_Backpack.transform.scale);

        // -- translate the backpack
        glm::vec3 physicsPos = GetFirstColliderPosition(m_PhysicsObjects);
        m_Backpack.SetPosition(physicsPos);
        model = glm::translate(model, m_Backpack.transform.position);
        m_MainShader.setUniformMat4(model, "model");

        // -- draw mesh
        m_Backpack.m_Model->Draw(m_MainShader);

        //RENDER SECOND OBJECT

        model = glm::mat4(1.0f);
        m_Backpack.SetScale(glm::vec3(5.25, 3.25, 2.25));
        model = glm::scale(model, m_Backpack.transform.scale);
        m_MainShader.setUniformMat4(model, "model");

        // -- draw mesh
        m_Backpack.m_Model->Draw(m_MainShader);

        // RENDER THIRD OBJECT

        model = glm::mat4(1.0f);
        m_Backpack.SetScale(glm::vec3(1.25, 1.25, 1.25));
        model = glm::scale(model, m_Backpack.transform.scale);
        m_MainShader.setUniformMat4(model, "model");

        // -- draw mesh
        m_Backpack.m_Model->Draw(m_MainShader);


        // render skybox after rendering rest of the scene (only draw skybox where an object is not present)
        GLCall(glDepthFunc(GL_LEQUAL)); // we change the depth function here to it passes when testingdepth value is equal to what is current stored
        m_Skybox.skyboxShader.use();
        view = glm::mat4(glm::mat3(m_Camera->getCameraView()));
        m_Skybox.skyboxShader.setUniformMat4(view, "view");
        m_Skybox.skyboxShader.setUniformMat4(projection, "projection");

        // -- render the skybox cube
        GLCall(glBindVertexArray(m_Skybox.skyboxVAO));
        GLCall(glActiveTexture(GL_TEXTURE0)); //! could be the problem
        GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_Skybox.cubeMapTexture));
        GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
        GLCall(glBindVertexArray(0));
        GLCall(glDepthFunc(GL_LESS)); // set depth function back to normal
    }
}