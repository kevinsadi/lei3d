#pragma once

#include "Camera.hpp"

struct GLFWwindow;

namespace lei3d
{
	class Application;
	class Camera;

	class FlyCamera : public Camera
	{
	private:
		const float MIN_FLY_SPEED = 1.0f;
		const float MAX_FLY_SPEED = 200.0f;
		const float MAX_PITCH = 89.0f;
		const float MOUSE_SENSITIVITY = 0.1f;

		float m_FlySpeed;

		bool m_MouseEnterFlag;
		bool m_UseMinecraftControls;

		int m_PrevX;
		int m_PrevY;

	public:
		FlyCamera(Entity& entity);
		~FlyCamera();

		void Init(GLFWwindow* window, float yaw, float pitch, float flySpeed);

		void OnImGuiRender() override;
		void cameraMouseCallback(double xPosInput, double yPosInput) override;
		void PollCameraMovementInput() override;

	private:
		void handleForward(float speed);
		void handleLeft(float speed);
		void handleRight(float speed);
		void handleBack(float speed);
		void handleUp(float speed);
		void handleDown(float speed);
	};
} // namespace lei3d