#include "scenes/Scene.hpp"

#include "logging/GLDebug.hpp"

namespace lei3d {
	Scene::Scene() {

	}

	Scene::~Scene() {
        Destroy();
	}

    void Scene::Init(Application* runningApp) {
        m_App = runningApp;

        // load camera
        GLFWwindow* const win = window();
        m_Camera = std::make_unique<FlyCamera>(win, 90.0f, 0.0f, 4.0f);

        Start();
    }

    void Scene::Start() {
        LEI_TRACE("Scene Start");
        LoadObjects();

        for (auto& entity : m_Entities) {
            entity->Start();
        }
    }

	void Scene::Update(float deltaTime) {
        //m_VP = m_Camera->GetProj() * m_Camera->GetView();

        //LEI_TRACE("Scene Update");
		for (auto& entity : m_Entities) {
			entity->Update(deltaTime);
		}

		OnUpdate(deltaTime);
	}

    void Scene::PhysicsUpdate(float deltaTime) {
        //LEI_TRACE("Scene Physics Update");
        for (auto& entity : m_Entities) {
            entity->PhysicsUpdate(deltaTime);
        }

        OnPhysicsUpdate(deltaTime);
    }

	void Scene::Render() {
        GLCall(glClearColor(0.2f, 0.8f, 0.9f, 1.0f));
        GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        //LEI_TRACE("Scene Render");
		for (auto& entity : m_Entities) {
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

    FlyCamera& Scene::MainCamera() {
        return *m_Camera;
    }

    PhysicsWorld& Scene::GetPhysicsWorld() {
        return *m_PhysicsWorld;
    }

    GLFWwindow* Scene::window() {
        return m_App->Window();
    }
}

