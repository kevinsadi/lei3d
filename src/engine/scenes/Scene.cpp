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
        m_Camera = std::make_unique<FlyCamera>(win, 90.0f, 0.0f, 10.0f);

        //Load shader (TEMPORARY)
        m_MainShader = std::make_unique<Shader>("./data/shaders/transformations.vert", "./data/shaders/transformations.frag");

        //Load physics world
        m_PhysicsWorld = std::make_unique<PhysicsWorld>();
        m_PhysicsWorld->Create();    //TODO: Consider if there is some better way to do this

        Start();
    }

    Entity& Scene::AddEntity(std::string name)
    {
        //Add number to name if multiple instances of the same name.
        std::string entityName = name;
        if (m_EntityNameCounts.contains(name))
        {
            std::string numberStr = std::to_string(m_EntityNameCounts[name]);
            entityName.append(std::format(" {0}", numberStr));
        }
        else
        {
            m_EntityNameCounts[name] = 0;
        }
        m_EntityNameCounts[name]++;

        std::unique_ptr<Entity> newEntity = std::make_unique<Entity>(entityName);
        m_Entities.push_back(std::move(newEntity));
        return *m_Entities.back();
    }

    Entity& Scene::AddEntity()
    {
        return AddEntity("Unnamed Entity");
    }

    Entity* Scene::GetEntity(std::string name)
    {
        for (auto& entity : m_Entities)
        {
            if (entity->GetName().compare(name) == 0)
            {
                return entity.get();
            }
        }

        return nullptr;
    }


    void Scene::Load()
    {
        //We might want to do general scene loading things here later.
        OnLoad();
    }

    void Scene::Unload()
    {
        m_Entities.clear(); //This should auto-destruct entities bc smart pointers.

        OnUnload();
    }

    void Scene::Start() {
        LEI_TRACE("Scene Start");
        Load();

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

        m_Camera->PollCameraMovementInput(deltaTime);
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

    void Scene::ImGUIRender()
    {
        ImGui::Text("Camera: ");
        m_Camera->OnImGuiRender();
        OnImGUIRender();
    }

    void Scene::Destroy() {
        LEI_TRACE("Scene Destroy");

        OnDestroy();
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

    void Scene::PrintEntityList()
    {
        for (auto& entity : m_Entities)
        {
            LEI_INFO("Entity: {0}", entity->GetName());
        }
    }
}

