#include "FlyCamera.hpp"

#include "core/Application.hpp"

namespace lei3d
{
	FlyCamera::FlyCamera(GLFWwindow* window, float yaw, float pitch, float flySpeed)
		: Camera(window, yaw, pitch)
	{
		m_FlySpeed = DEFAULT_FLY_SPEED;
	}

	FlyCamera::~FlyCamera()
	{
		// no need rn
	}

	void FlyCamera::PollCameraMovementInput()
	{
		GLFWwindow* const window = Application::Window();

		float speed = m_FlySpeed;
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			speed *= 10.0f;

			if (speed > MAX_FLY_SPEED)
			{
				speed = MAX_FLY_SPEED;
			}
		}

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			handleForward(speed);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			handleBack(speed);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			handleLeft(speed);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			handleRight(speed);
		}
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		{
			handleUp(speed);
		}
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			handleDown(speed);
		}
	}

	void FlyCamera::OnImGuiRender()
	{
		ImGui::SliderFloat("Camera Speed", &m_FlySpeed, MIN_FLY_SPEED, MAX_FLY_SPEED, "%.2f");
		ImGui::Checkbox("Use 'Minecraft' Camera Controls: ", &m_UseMinecraftControls);
	}

	void FlyCamera::handleForward(float speed)
	{
		m_CameraPos += m_CameraFront * speed * Application::DeltaTime();

		if (m_UseMinecraftControls) {
			//should move in m_CameraFront's direction, with no change in height
			m_CameraPos += glm::normalize(glm::vec3(m_CameraFront.x, 0, m_CameraFront.z)) * speed * Application::DeltaTime();
		} else {
			m_CameraPos += m_CameraFront * speed * Application::DeltaTime();
		}
	}

	void FlyCamera::handleBack(float speed)
	{
		m_CameraPos -= m_CameraFront * speed * Application::DeltaTime();

		if (m_UseMinecraftControls) {
			//should move opposite to m_CameraFront's direction, with no change in height
			m_CameraPos -= glm::normalize(glm::vec3(m_CameraFront.x, 0, m_CameraFront.z)) * speed * Application::DeltaTime();
		} else {
			m_CameraPos -= m_CameraFront * speed * Application::DeltaTime();
		}
	}

	void FlyCamera::handleLeft(float speed)
	{
		m_CameraPos -= m_CameraRight * speed * Application::DeltaTime();
	}

	void FlyCamera::handleRight(float speed)
	{
		m_CameraPos += m_CameraRight * speed * Application::DeltaTime();
	}

	void FlyCamera::handleUp(float speed)
	{
		m_CameraPos += m_CameraUp * speed * Application::DeltaTime();
	}

	void FlyCamera::handleDown(float speed)
	{
		m_CameraPos -= m_CameraUp * speed * Application::DeltaTime();
	}

	void FlyCamera::SetFlySpeed(float speed) {
		m_FlySpeed = speed;
	}
} // namespace lei3d