#pragma once

#include "Camera.hpp"

struct GLFWwindow;

namespace lei3d
{
	class Application;
	class Camera;

	/*
	*  Camera with flying/no-clip controls.
	*/
	class FlyCamera : public Camera
	{
	private:
		constexpr static float MIN_FLY_SPEED = 1.0f;
		constexpr static float MAX_FLY_SPEED = 200.0f;
		constexpr static float DEFAULT_FLY_SPEED = 40.0f;

		float m_FlySpeed;

		bool m_UseMinecraftControls;
	public:
		FlyCamera(GLFWwindow* window, float yaw = 0.0f, float pitch = 0.0f, float flySpeed = DEFAULT_FLY_SPEED);
		~FlyCamera();

		void OnImGuiRender() override;
		void PollCameraMovementInput() override;

		void SetFlySpeed(float speed);

	private:
		void handleForward(float speed);
		void handleLeft(float speed);
		void handleRight(float speed);
		void handleBack(float speed);
		void handleUp(float speed);
		void handleDown(float speed);
	};
} // namespace lei3d