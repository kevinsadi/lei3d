#include "core/Scene.hpp"

#include "core/Application.hpp"
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
		// load camera
		GLFWwindow* const win = Application::Window();
		m_Camera = std::make_unique<FirstPersonCamera>(win, 90.0f, 0.0f, 10.0f);

		// Load shader (TEMPORARY)
		m_MainShader = std::make_unique<Shader>("./data/shaders/transformations.vert", "./data/shaders/transformations.frag");

		// Load physics world
		m_PhysicsWorld = std::make_unique<PhysicsWorld>();
		m_PhysicsWorld->Create(); // TODO: Consider if there is some better way to do this

		OnLoad();

		m_State = SCENE_PLAYING;
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
	}

	void Scene::Play()
	{
		m_State = SCENE_PLAYING;
	}

	void Scene::Pause()
	{
		m_State = SCENE_PAUSED;
	}

	void Scene::Reset()
	{
		//We have to do some tricky things to get the scene to reset all the objs and physics and stuff.
		m_State = SCENE_START;
		Load(); //This is inefficient
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
		if (m_State == SCENE_PLAYING)
		{
			//LEI_TRACE("Scene Update");

			for (auto& entity : m_Entities)
			{
				entity->Update();
			}

			OnUpdate();
		}

		m_Camera->PollCameraMovementInput();
	}

	void Scene::PhysicsUpdate()
	{
		if (m_State == SCENE_PLAYING)
		{
			//LEI_TRACE("Scene Physics Update");
			for (auto& entity : m_Entities)
			{
				entity->PhysicsUpdate();
			}

			OnPhysicsUpdate();
		}
	}

	void Scene::Render()
	{
		GLCall(glClearColor(0.2f, 0.8f, 0.9f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		// LEI_TRACE("Scene Render");
		for (auto& entity : m_Entities)
		{
			entity->Render();
		}

		OnRender();
	}

	//yucky
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

	void Scene::ImGUIRender()
	{
		//Scene Control Widgets
		std::stringstream ss;
		ss << "State: ";
		ss << StateToString();
		ImGui::Text(ss.str().c_str());

		if (ImGui::Button("Play"))
		{
			Play();
		}

		ImGui::SameLine();
		if (ImGui::Button("Pause"))
		{
			Pause();
		}

		//ImGui::SameLine();
		//if (ImGui::Button("Reset"))
		//{
		//    Reset();
		//}

		ImGui::Text("Camera: ");
		m_Camera->OnImGuiRender();

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
					const bool	is_selected = (currentEntityI == i);
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

		OnImGUIRender();
	}

	void Scene::Destroy()
	{
		LEI_TRACE("Scene Destroy");

		OnDestroy();
	}

	FirstPersonCamera& Scene::MainCamera() const
	{
		return *m_Camera;
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
