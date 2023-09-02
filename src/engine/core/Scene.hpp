#pragma once

#include "core/Entity.hpp"
#include "core/FlyCamera.hpp"
#include "core/FirstPersonCamera.hpp"
#include "physics/PhysicsWorld.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <unordered_map>
#include <vector>

namespace lei3d
{
	class Application;
	class Entity;
	class FirstPersonCamera;
	class PhysicsWorld;
	class Shader;
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
		//We should prob. limit how much stuff we put into the base scene.
		std::unique_ptr<FirstPersonCamera> m_Camera = nullptr;		 // every scene needs a camera
		std::unique_ptr<Shader>			   m_MainShader = nullptr;	 // THIS IS TEMPORARY
		std::unique_ptr<PhysicsWorld>	   m_PhysicsWorld = nullptr; // Each scene has a physics world

		SceneState m_State;

	public:
		Scene();
		~Scene();	

		//Entities
		Entity& AddEntity(std::string name);
		Entity& AddEntity();

		// Entity Messages
		void Start();
		void Update();
		void PhysicsUpdate();
		void Render();
		void ImGUIRender();
		void Destroy();

		//Scene State Changers
		void Play();
		void Pause();
		void Reset();

		void Load();
		void Unload();

		// TODO: Abstract scene creation/loading into files: https://trello.com/c/eC66QGuD/25-define-scene-file-format
		// Right now we use this virtual Load function to load all the objs in code.
		virtual void OnLoad() {}
		virtual void OnUnload() {}

		// These should rarely be used because everything is handled by ECS.
		virtual void OnStart() {}
		virtual void OnUpdate() {}
		virtual void OnPhysicsUpdate() {}
		virtual void OnRender() {}
		virtual void OnImGUIRender() {}
		virtual void OnDestroy() {}

		Entity*			   GetEntity(std::string name) const;
		FirstPersonCamera& MainCamera() const;
		PhysicsWorld&	   GetPhysicsWorld() const;

		void PrintEntityList() const; // For Debugging

	private:
		std::string StateToString() const;
	};
} // namespace lei3d
