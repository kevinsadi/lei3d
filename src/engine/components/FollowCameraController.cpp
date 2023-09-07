#include "FollowCameraController.hpp"

#include "core/Application.hpp"

namespace lei3d
{
	FollowCameraController::FollowCameraController(Entity& entity)
		: Component(entity), m_FollowEntity{&entity}
	{
	}

	FollowCameraController::~FollowCameraController()
	{
		// no need rn
	}

	void FollowCameraController::Init(Camera& camera, glm::vec3 offsetFromEntity)
	{
		m_Camera = &camera;
		m_OffsetFromEntity = offsetFromEntity;
	}

	void FollowCameraController::OnImGuiRender()
	{
	}

	void FollowCameraController::Update()
	{
		//Modify the player's lookdir to match the camera look
		m_FollowEntity->m_Transform.yawRotation = m_Camera->GetYaw();
		m_Camera->SetPosition(m_FollowEntity->m_Transform.position + m_OffsetFromEntity);
	}

	Camera* FollowCameraController::GetCamera() const
	{
		return m_Camera;
	}
} // namespace lei3d