#include "include/scenes/Scene.hpp"

namespace lei3d {
	Scene::Scene() {

	}

	Scene::~Scene() {
        if (m_Camera)
            delete m_Camera;
	}

    void Scene::Init(Application* runningApp) {
        m_App = runningApp;
        m_PhysicsObjects = CreatePhysicsWorld();
        initCamera();
    }

	void Scene::Update(float deltaTime) {
		for (auto entity : m_Entities) {
			entity->Update(deltaTime);
		}

        PhysicsStep(m_PhysicsObjects, deltaTime);

		OnUpdate();
	}

	void Scene::Render() {
		for (auto entity : m_Entities) {
			entity->Render();
		}

		OnRender();
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

    void Scene::initCamera() {
        
        // load camera
        GLFWwindow* const win = window();
        m_Camera = new FlyCamera(win, 90.0f, 0.0f, 4.0f);
    }

    FlyCamera* Scene::MainCamera() {
        return m_Camera;
    }

    GLFWwindow* Scene::window() {
        return m_App->Window();
    }


}

