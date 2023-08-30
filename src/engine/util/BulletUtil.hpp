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
	inline glm::vec3 btTransformToVec3(const btTransform& trans)
	{
		const btVector3& origin = trans.getOrigin();
		return glm::vec3(float(origin.getX()), float(origin.getY()), float(origin.getZ()));
	}
} // namespace lei3d