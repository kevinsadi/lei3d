#pragma once

#include "Camera.hpp"

#include "core/Entity.hpp"

struct GLFWwindow;

namespace lei3d
{
	class Application;
	class Entity;

	class FirstPersonCamera : public Camera
	{
	private:
		const float MAX_PITCH = 89.0f;
		const float MOUSE_SENSITIVITY = 0.1f;

		bool m_MouseEnterFlag;

		int m_PrevX;
		int m_PrevY;

		Entity*	  m_FollowEntity;
		glm::vec3 m_OffsetFromEntity;

	public:
		FirstPersonCamera(Entity& entity);
		~FirstPersonCamera();

		void Init(GLFWwindow* window, float yaw, float pitch, Entity* followEntity, glm::vec3 offsetFromEntity);

		void OnImGuiRender() override;
		void cameraMouseCallback(double xPosInput, double yPosInput) override;
		void PollCameraMovementInput() override;

		void Update() override;

	private:
		void	  RotateEntity(float xoffset);
		glm::vec3 GetPlayerPos() const;
	};
} // namespace lei3d