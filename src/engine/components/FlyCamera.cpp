#include "FlyCamera.hpp"

#include "core/Application.hpp"

namespace lei3d
{
	FlyCamera::FlyCamera(Entity& entity)
		: Camera(entity)
	{
	}

	FlyCamera::~FlyCamera()
	{
		// no need rn
	}

	void FlyCamera::Init(GLFWwindow* window, float yaw, float pitch, float flySpeed)
	// this is a member initializer list using uniform initialization C++17 feature
	{
		Camera::Init(window, yaw, pitch);
		m_FlySpeed = flySpeed;

		m_MouseEnterFlag = true; // the first time the mouse is entered onto the screen, do not flick

		int screenWidth, screenHeight;
		glfwGetWindowSize(window, &screenWidth, &screenHeight);
		m_PrevX = screenWidth / 2.0f;
		m_PrevY = screenHeight / 2.0f;
		m_Aspect = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);

		m_CameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		m_CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		m_CameraRight = glm::vec3(1.0f, 0.0f, 0.0f);
	}

	void FlyCamera::cameraMouseCallback(double xPosInput, double yPosInput)
	{
		float xpos = static_cast<float>(xPosInput);
		float ypos = static_cast<float>(yPosInput);

		if (m_MouseEnterFlag)
		{
			m_PrevX = xpos;
			m_PrevY = ypos;
			m_MouseEnterFlag = false;
		}

		float xoffset = xpos - m_PrevX;
		float yoffset = m_PrevY - ypos; // openGL inverted y
		m_PrevX = xpos;
		m_PrevY = ypos;

		xoffset *= MOUSE_SENSITIVITY;
		yoffset *= MOUSE_SENSITIVITY;

		m_Yaw += xoffset;
		m_Pitch += yoffset;

		if (m_Pitch > MAX_PITCH)
			m_Pitch = MAX_PITCH;
		if (m_Pitch < -MAX_PITCH)
			m_Pitch = -MAX_PITCH;

		glm::vec3 direction;
		direction.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		direction.y = sin(glm::radians(m_Pitch));
		direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_CameraFront = glm::normalize(direction);
		m_CameraRight = glm::normalize(glm::cross(m_CameraFront, m_CameraUp));
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
		ImGui::SliderFloat("Camera Speed: ", &m_FlySpeed, MIN_FLY_SPEED, MAX_FLY_SPEED, "%.2f");
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
} // namespace lei3d