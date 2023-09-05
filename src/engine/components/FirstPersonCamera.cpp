#include "FirstPersonCamera.hpp"

#include "core/Application.hpp"

namespace lei3d
{
	FirstPersonCamera::FirstPersonCamera(Entity& entity)
		: Camera(entity)
	{
	}

	FirstPersonCamera::~FirstPersonCamera()
	{
		// no need rn
	}

	void FirstPersonCamera::Init(GLFWwindow* window, float yaw, float pitch, Entity* followEntity, glm::vec3 offsetFromEntity)
	{
		Camera::Init(window, yaw, pitch);
		m_FollowEntity = followEntity;
		m_OffsetFromEntity = offsetFromEntity;

		m_MouseEnterFlag = true; // the first time the mouse is entered onto the screen, do not flick

		int screenWidth, screenHeight;
		glfwGetWindowSize(window, &screenWidth, &screenHeight);
		m_PrevX = screenWidth / 2.0f;
		m_PrevY = screenHeight / 2.0f;
		m_Aspect = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);

		m_CameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		m_CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	}

	void FirstPersonCamera::cameraMouseCallback(double xPosInput, double yPosInput)
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

		m_Pitch += yoffset;
		m_Yaw += xoffset;
		RotateEntity(xoffset);

		if (m_Pitch > MAX_PITCH)
			m_Pitch = MAX_PITCH;
		if (m_Pitch < -MAX_PITCH)
			m_Pitch = -MAX_PITCH;

		glm::vec3 direction;
		direction.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		direction.y = sin(glm::radians(m_Pitch));
		direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_CameraFront = glm::normalize(direction);
	}

	void FirstPersonCamera::PollCameraMovementInput()
	{
		// GLFWwindow* const window = Application::Window();

		// float speed = m_FlySpeed;
		// if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		//{
		//	speed *= 10.0f;

		//	if (speed > MAX_FLY_SPEED)
		//	{
		//		speed = MAX_FLY_SPEED;
		//	}
		//}
	}

	void FirstPersonCamera::OnImGuiRender()
	{
		// ImGui::SliderFloat("Camera Speed: ", &m_FlySpeed, MIN_FLY_SPEED, MAX_FLY_SPEED, "%.2f");
	}

	void FirstPersonCamera::Update()
	{
		SetPosition(m_FollowEntity->m_Transform.position + m_OffsetFromEntity);
	}

	void FirstPersonCamera::RotateEntity(float xoffset)
	{
		if (m_FollowEntity)
		{
			m_FollowEntity->m_Transform.yawRotation += xoffset;
		}
		else
		{
			LEI_WARN("FPS Camera Not Attached to Entity when calling 'Rotate'");
			LEI_WARN("FirstPersonCamera: Unable to find Player Entity, ensure it is named correctly in `RotatePlayer`");
		}
	}

	glm::vec3 FirstPersonCamera::GetPlayerPos() const
	{
		if (m_FollowEntity)
		{
			return m_FollowEntity->m_Transform.position;
		}

		LEI_WARN("FPS Camera not attached to an entity");
		return glm::vec3(0.0f, 0.0f, 0.0f);
	}

} // namespace lei3d