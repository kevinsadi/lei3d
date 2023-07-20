#include "scenes/Scene.hpp"

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
        m_Camera = new FlyCamera(win, 90.0f, 0.0f, 4.0f);

        Start();
    }

    void Scene::Start() {
        LEI_TRACE("Scene Start");
        LoadObjects();

        for (Entity& entity : m_Entities) {
            entity.Start();
        }
    }

	void Scene::Update(float deltaTime) {
        //LEI_TRACE("Scene Update");
		for (Entity& entity : m_Entities) {
			entity.Update(deltaTime);
		}

		OnUpdate(deltaTime);
	}

    void Scene::PhysicsUpdate(float deltaTime) {
        //LEI_TRACE("Scene Physics Update");
        for (Entity& entity : m_Entities) {
            entity.PhysicsUpdate(deltaTime);
        }

        PhysicsStep(m_PhysicsObjects, deltaTime);
    }

	void Scene::Render() {
        //LEI_TRACE("Scene Render");
		for (Entity& entity : m_Entities) {
			entity.Render();
		}

		OnRender();
	}

    void Scene::Destroy() {
        LEI_TRACE("Scene Destroy");

        if (m_Camera) {
            delete m_Camera;
        }

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

    FlyCamera* Scene::MainCamera() {
        return m_Camera;
    }

    GLFWwindow* Scene::window() {
        return m_App->Window();
    }
}

