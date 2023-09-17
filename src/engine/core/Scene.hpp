#pragma once

#include "core/Entity.hpp"

#include "core/Camera.hpp"

#include "physics/PhysicsWorld.hpp"
#include "components/Lights.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <unordered_map>
#include <vector>

namespace lei3d
{
	class Entity;
	class Camera;
	class PhysicsWorld;
	class RenderSystem;

	class Scene
	{
		enum SceneState
		{
			SCENE_PLAYING,
			SCENE_PAUSED,
			SCENE_START,
		};

	private:
		friend RenderSystem;

		std::vector<std::unique_ptr<Entity>> m_Entities;
		std::unordered_map<std::string, int> m_EntityNameCounts;

	protected:
		// We should prob. limit how much stuff we put into the base scene.

		//FlyCamera moved to SceneView
		std::unique_ptr<Camera>			  m_DefaultCamera = nullptr;
		std::unique_ptr<PhysicsWorld>	  m_PhysicsWorld = nullptr; // Each scene has a physics world
		std::unique_ptr<DirectionalLight> m_DirectionalLight = nullptr;

		SceneState m_State;

	public:
		Scene();
		~Scene();

		// Entities
		Entity& AddEntity(std::string name);
		Entity& AddEntity();

		// Entity Messages
		void Start();
		void Update();
		void PhysicsUpdate();
		void Destroy();

		// Scene State Changers
		void Play();
		void Pause();
		void Reset();

		void Load();
		void Unload();

		//GUI
		void ShowHeirarchyGUI();

		// TODO: Abstract scene creation/loading into files: https://trello.com/c/eC66QGuD/25-define-scene-file-format
		// Right now we use this virtual Load function to load all the objs in code.
		virtual void OnLoad() {}
		virtual void OnUnload() {}

		// These should rarely be used because everything is handled by ECS.
		virtual void OnStart() {}
		virtual void OnUpdate() {}
		virtual void OnPhysicsUpdate() {}
		virtual void OnDestroy() {}
		virtual void OnReset() {}

		virtual Camera& GetMainCamera() const; //Scene must have some camera created (basically just the first person camera lmao.

		Entity*		  GetEntity(std::string name) const;
		PhysicsWorld& GetPhysicsWorld() const;

		void PrintEntityList() const; // For Debugging

		std::string StateToString() const;
	};
} // namespace lei3d
