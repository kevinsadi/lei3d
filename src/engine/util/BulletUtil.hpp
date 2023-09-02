#pragma once

#include <btBulletDynamicsCommon.h>

#include <glm/glm.hpp>

namespace lei3d
{
	inline btVector3 glmToBTVec3(const glm::vec3& v)
	{
		return btVector3{
			v.x, v.y, v.z
		};
	}

	inline glm::vec3 btToGLMVec3(const btVector3& v)
	{
		return {v.getX(), v.getY(), v.getZ()};
	}

	inline glm::vec3 btTransformToVec3(const btTransform& trans)
	{
		const btVector3& origin = trans.getOrigin();
		return { float(origin.getX()), float(origin.getY()), float(origin.getZ()) };
	}
} // namespace lei3d