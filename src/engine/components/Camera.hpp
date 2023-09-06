#pragma once

#include "core/Component.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

struct GLFWwindow;

namespace lei3d
{
	class Application;
	class Component;

	class Camera : public Component
	{
	protected:
		float m_Yaw;
		float m_Pitch;
		// We don't roll around here

		// projection values
		float m_FOVDeg, m_Aspect, m_NearPlane, m_FarPlane;

		glm::vec3 m_CameraPos;
		glm::vec3 m_CameraFront;
		glm::vec3 m_CameraUp;
		glm::vec3 m_CameraRight;

	public:
		Camera(Entity& entity);
		~Camera();

		void Init(GLFWwindow* window, float yaw, float pitch);

		glm::mat4 GetView();
		glm::mat4 GetProj();
		glm::vec3 GetPosition() const;
		glm::vec3 GetFront() const;
		glm::vec3 GetUp() const;
		float GetFOV() const;

		virtual void OnImGuiRender() {}
		virtual void cameraMouseCallback(double xPosInput, double yPosInput) {}
		virtual void PollCameraMovementInput() {}

		void SetFOV(float fov);
		void SetClipPlanes(float nearClip, float farClip);
		void SetPosition(glm::vec3 pos);
	};
} // namespace lei3d