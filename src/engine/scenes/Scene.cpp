#include "scenes/Scene.hpp"

#include "util/GLDebug.hpp"

namespace lei3d {
	Scene::Scene() {

	}

	Scene::~Scene() {
        Destroy();
	}

    void Scene::Init(Application* runningApp) {
        m_App = runningApp;
        m_PhysicsObjects = CreatePhysicsWorld();

        // load camera
        GLFWwindow* const win = window();
        m_Camera = std::make_shared<FlyCamera>(win, 90.0f, 0.0f, 4.0f);

        Start();
    }

    void Scene::Start() {
        LEI_TRACE("Scene Start");
        LoadObjects();

        for (auto entity : m_Entities) {
            entity->Start();
        }
    }

	void Scene::Update(float deltaTime) {
        //LEI_TRACE("Scene Update");
		for (auto entity : m_Entities) {
			entity->Update(deltaTime);
		}

		OnUpdate(deltaTime);
	}

    void Scene::PhysicsUpdate(float deltaTime) {
        //LEI_TRACE("Scene Physics Update");
        for (auto entity : m_Entities) {
            entity->PhysicsUpdate(deltaTime);
        }

        PhysicsStep(m_PhysicsObjects, deltaTime);
    }

	void Scene::Render() {
        GLCall(glClearColor(0.2f, 0.8f, 0.9f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        //Update view/projection matrix (generally on the gpu this is just MVP for the whole thing)
        //Idk how to do shader updates yet. Prob. we will want to have a set of static default shaders managed somewhere
        //that we update using the scene info.
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 400.0f);
        m_MainShader->setUniformMat4(projection, "proj");

        glm::mat4 view = m_Camera->getCameraView();
        m_MainShader->setUniformMat4(view, "view");

        //LEI_TRACE("Scene Render");
		for (auto entity : m_Entities) {
			entity->Render();
		}

        OnRender();
	}

    void Scene::Destroy() {
        LEI_TRACE("Scene Destroy");

        OnDestroy();
    }

    void Scene::ProcessCameraInput(float deltaTime)
    {
        GLFWwindow* const window = m_App->Window();

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            m_Camera->handleForward(deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            m_Camera->handleBack(deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            m_Camera->handleLeft(deltaTime);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            m_Camera->handleRight(deltaTime);
        }
    }

    std::shared_ptr<FlyCamera> Scene::MainCamera() {
        return m_Camera;
    }

    GLFWwindow* Scene::window() {
        return m_App->Window();
    }
}

