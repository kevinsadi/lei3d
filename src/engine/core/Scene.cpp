#include "core/Scene.hpp"

#include "InputManager.hpp"
#include "core/Application.hpp"
#include "core/Camera.hpp"

#include "logging/GLDebug.hpp"

namespace lei3d
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
		Destroy();
	}

	void Scene::Load()
	{
		// Default Camera
		m_DefaultCamera = std::make_unique<Camera>(Application::Window(), 90.0f, 0.0f);

		// Load physics world
		m_PhysicsWorld = std::make_unique<PhysicsWorld>();
		m_PhysicsWorld->Create(); // TODO: Consider if there is some better way to do this

		// Default light, TODO: needs to load from scene file
		DirectionalLight* dirLight = new DirectionalLight({ 0.1, -0.5, -0.45 }, { 1.f, 0.97f, 0.757f }, 3.f);
		m_DirectionalLight = std::unique_ptr<DirectionalLight>(dirLight);

		OnLoad();

		m_State = SCENE_START; // Scene ready to start by default.
	}

	Entity& Scene::AddEntity(std::string name)
	{
		// Add number to name if multiple instances of the same name.
		std::stringstream entityNameSS;
		entityNameSS << name;
		if (m_EntityNameCounts.find(name) != m_EntityNameCounts.end())
		{
			const std::string numberStr = std::to_string(m_EntityNameCounts[name]);
			entityNameSS << numberStr;
		}
		else
		{
			m_EntityNameCounts[name] = 0;
		}
		m_EntityNameCounts[name]++;

		std::unique_ptr<Entity> newEntity = std::make_unique<Entity>(entityNameSS.str());
		m_Entities.push_back(std::move(newEntity));
		return *m_Entities.back();
	}

	Entity& Scene::AddEntity()
	{
		return AddEntity("Unnamed Entity");
	}

	void RemoveEntity(std::string)
	{
	}

	Entity* Scene::GetEntity(std::string name) const
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

	void Scene::Unload()
	{
		m_Entities.clear(); // This should auto-destruct entities bc smart pointers.

		OnUnload();
		Destroy();
	}

	void Scene::Play()
	{
		if (m_State == SCENE_START)
		{
			// Initialize everything if we're starting the scene, otherwise no.
			Start();
		}
		m_State = SCENE_PLAYING;

		InputManager::GetInstance().giveInputFocus(InputManager::InputTarget::GAME);
	}

	void Scene::Pause()
	{
		m_State = SCENE_PAUSED;
	}

	void Scene::Reset()
	{
		// We have to do some tricky things to get the scene to reset all the objs and physics and stuff.
		m_State = SCENE_START;

		OnReset();
		for (auto& entity : m_Entities)
		{
			entity->OnReset();
		}
	}

	void Scene::Start()
	{
		LEI_TRACE("Scene Start");

		for (auto& entity : m_Entities)
		{
			entity->Start();
		}

		OnStart();
	}

	void Scene::Update()
	{
		switch (m_State)
		{
			case SCENE_PLAYING:
				// LEI_TRACE("Scene Update");

				for (auto& entity : m_Entities)
				{
					entity->Update();
				}

				OnUpdate();
				break;
			case SCENE_PAUSED:
			case SCENE_START:
				break;
		}
	}

	void Scene::PhysicsUpdate()
	{
		if (m_State == SCENE_PLAYING)
		{
			// LEI_TRACE("Scene Physics Update");
			for (auto& entity : m_Entities)
			{
				entity->PhysicsUpdate();
			}

			OnPhysicsUpdate();
		}
	}

	// yucky
	std::string Scene::StateToString() const
	{
		switch (m_State)
		{
			case SCENE_PLAYING:
				return "Playing";
			case SCENE_PAUSED:
				return "Paused";
			case SCENE_START:
			default:
				return "Ready To Start";
		}
	}

#ifdef EDITOR
	void Scene::ShowHeirarchyGUI()
	{
		bool* p_open;
		ImGuiWindowFlags window_flags = 0;

		// Comment/Uncomment these as needed
		p_open = NULL;
		// window_flags |= ImGuiWindowFlags_NoTitleBar;
		// window_flags |= ImGuiWindowFlags_NoScrollbar;
		window_flags |= ImGuiWindowFlags_MenuBar;
		// window_flags |= ImGuiWindowFlags_NoMove;
		// window_flags |= ImGuiWindowFlags_NoResize;
		// window_flags |= ImGuiWindowFlags_NoCollapse;
		// window_flags |= ImGuiWindowFlags_NoNav;
		// window_flags |= ImGuiWindowFlags_NoBackground;
		// window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
		// window_flags |= ImGuiWindowFlags_UnsavedDocument;

		if (!ImGui::Begin("Scene Heirarchy", p_open, window_flags))
		{
			// Early out if the window is collapsed, as an optimization.
			ImGui::End();
			return;
		}

		ImGui::Text("Physics World: ");
		m_PhysicsWorld->OnImGuiRender();

		static int currentEntityI = -1; // Here we store our selection data as an index.
		if (ImGui::TreeNode("Entities"))
		{
			// Using the generic BeginListBox() API, you have full control over how to display the combo contents.
			// (your selection data could be an index, a pointer to the object, an id for the object, a flag intrusively
			// stored in the object itself, etc.)
			std::vector<std::string> entityNames;
			for (auto& entity : m_Entities)
			{
				entityNames.push_back(entity->GetName());
			}

			// LEI_INFO("Number of Entities: {0}", entityNames.size());

			if (ImGui::BeginListBox("Entities"))
			{
				for (int i = 0; i < entityNames.size(); i++)
				{
					const char* label = entityNames[i] == "" ? "Unnamed" : entityNames[i].c_str();
					const bool is_selected = (currentEntityI == i);
					if (ImGui::Selectable(label, is_selected))
					{
						currentEntityI = i;
					}

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndListBox();
			}

			ImGui::TreePop();
		}

		// Create an inspector for the entity if one is selected
		if (currentEntityI >= 0)
		{
			Entity& entity = *m_Entities[currentEntityI];
			entity.ShowInspectorGUI();
		}

		ImGui::SetWindowSize(ImVec2(300, 600), ImGuiCond_Once);
		ImGui::SetWindowPos(ImVec2(0, 500), ImGuiCond_Once);
		ImGui::End();
	}
#endif

	void Scene::Destroy()
	{
		LEI_TRACE("Scene Destroy");

		m_PhysicsWorld.reset();
		m_DirectionalLight.reset();

		OnDestroy();
	}

	Camera& Scene::GetMainCamera() const
	{
		return *m_DefaultCamera;
	}

	PhysicsWorld& Scene::GetPhysicsWorld() const
	{
		return *m_PhysicsWorld;
	}

	void Scene::PrintEntityList() const
	{
		for (auto& entity : m_Entities)
		{
			LEI_INFO("Entity: {0}", entity->GetName());
		}
	}
} // namespace lei3d
