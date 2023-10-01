#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

struct GLFWwindow;

namespace lei3d
{
	class Camera
	{
	public:
		enum LookMode
		{
			FREE,
			FIXED,
		};

	protected:
		const float MAX_PITCH = 89.0f;
		const float MOUSE_SENSITIVITY = 0.1f;

		float m_Yaw;
		float m_Pitch;
		// We don't roll around here

		// projection values
		float m_FOVDeg, m_Aspect, m_NearPlane, m_FarPlane;

		glm::vec3 m_CameraPos;
		glm::vec3 m_CameraFront;
		glm::vec3 m_CameraUp;
		glm::vec3 m_CameraRight;

		LookMode m_LookMode;

		bool m_MouseEnterFlag;

		int m_PrevX;
		int m_PrevY;

	public:
		Camera(GLFWwindow* window, float yaw = 0.0f, float pitch = 0.0f, LookMode mode = LookMode::FREE);
		~Camera();

		glm::mat4 GetView();
		glm::mat4 GetProj();
		glm::vec3 GetPosition() const;
		glm::vec3 GetFront() const;
		glm::vec3 GetUp() const;
		float GetFOV() const;
		float GetYaw() const;
		float GetPitch() const;

		float GetNearPlane() const;
		float GetFarPlane() const;

		virtual void OnImGuiRender() {}
		virtual void cameraMouseCallback(double xPosInput, double yPosInput);
		virtual void PollCameraMovementInput() {}

		void SetFOV(float fov);
		void SetClipPlanes(float nearClip, float farClip);
		void SetPosition(glm::vec3 pos);

	private:
		void FreeCameraControls(float xpos, float ypos);
	};
} // namespace lei3d