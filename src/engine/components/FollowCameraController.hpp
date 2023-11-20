#pragma once

#include "core/Camera.hpp"
#include "core/Component.hpp"
#include "core/Entity.hpp"

struct GLFWwindow;

namespace lei3d
{
	class Application;
	class Camera;
	class Component;
	class Entity;

	class FollowCameraController : public Component
	{
	private:
		//Camera follows something. By default, this is the entity the component is attached to
		Camera*	  m_Camera;	//This is pointer just so we don't need to init in constructor
		Entity*	  m_FollowEntity;
		glm::vec3 m_OffsetFromEntity;

	public:
		FollowCameraController(Entity& entity);
		~FollowCameraController();

		void Init(Camera& camera, glm::vec3 offsetFromEntity);

#ifdef EDITOR
		void OnImGuiRender() override;
#endif
		void Update() override;

		Camera* GetCamera() const;

	private:
		glm::vec3 GetPlayerPos() const;
	};
} // namespace lei3d