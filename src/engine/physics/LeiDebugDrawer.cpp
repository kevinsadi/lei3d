#include "LeiDebugDrawer.hpp"

#include "core/Application.hpp"

#include "util/BulletUtil.hpp"

namespace lei3d
{
	void LeiDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
	{
		Application::GetInstance().GetPrimitiveRenderer().pushLine(Application::GetInstance().GetSceneCamera(), btToGLMVec3(from), btToGLMVec3(to), btToGLMVec3(color), 0.5f);
	}

	int LeiDebugDrawer::getDebugMode() const
	{
		return m_DebugMode;
	}

	void LeiDebugDrawer::setDebugMode(int debugMode)
	{
		m_DebugMode = debugMode;
	}

	void LeiDebugDrawer::reportErrorWarning(const char* warningString)
	{
		LEI_ERROR("BULLET DEBUG ERROR: {0}", warningString);
	}

	void LeiDebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
	{
		if (distance < 0)
		{
			Application::GetInstance().GetPrimitiveRenderer().pushLine(Application::GetInstance().GetSceneCamera(), btToGLMVec3(PointOnB), btToGLMVec3((PointOnB + normalOnB.normalized() * 12)), btToGLMVec3(btVector3(1, 0, 0)), 0.5f);
		}
		else
		{
			Application::GetInstance().GetPrimitiveRenderer().pushLine(Application::GetInstance().GetSceneCamera(), btToGLMVec3(PointOnB), btToGLMVec3((PointOnB + normalOnB.normalized() * 12)), btToGLMVec3(btVector3(0, 1, 0)), 0.5f);
		}
	}

	void LeiDebugDrawer::draw3dText(const btVector3& location, const char* textString)
	{
	}
} // namespace lei3d