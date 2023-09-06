#include "Camera.hpp"

#include "core/Application.hpp"

namespace lei3d
{
	Camera::Camera(Entity& entity)
		: Component(entity)
	{
	}

	Camera::~Camera()
	{
		//: Clown Emoji
	}

	void Camera::Init(GLFWwindow* window, float yaw, float pitch)
	{
		m_Yaw = yaw;
		m_Pitch = pitch;

		m_FOVDeg = 45.0f;
		m_NearPlane = 0.1f;
		m_FarPlane = 700.0f;

		int screenWidth, screenHeight;
		glfwGetWindowSize(window, &screenWidth, &screenHeight);
		m_Aspect = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);

		m_CameraPos = glm::vec3(0.0f, 0.0f, 3.0f); // Why is this hard-coded as a 3?
		m_CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	}

	glm::mat4 Camera::GetView()
	{
		return glm::lookAt(m_CameraPos, m_CameraPos + m_CameraFront, m_CameraUp);
	}

	glm::mat4 Camera::GetProj()
	{
		return glm::perspective(glm::radians(m_FOVDeg), m_Aspect, m_NearPlane, m_FarPlane);
	}

	glm::vec3 Camera::GetPosition() const
	{
		return m_CameraPos;
	}

	glm::vec3 Camera::GetFront() const
	{
		return m_CameraFront;
	}

	glm::vec3 Camera::GetUp() const
	{
		return m_CameraUp;
	}

	float Camera::GetFOV() const
	{
		return m_FOVDeg;
	}

	void Camera::SetFOV(float fovDeg)
	{
		m_FOVDeg = fovDeg;
	}

	void Camera::SetClipPlanes(float nearClip, float farClip)
	{
		m_NearPlane = nearClip;
		m_FarPlane = farClip;
	}

	void Camera::SetPosition(glm::vec3 pos)
	{
		m_CameraPos = pos;
	}
} // namespace lei3d