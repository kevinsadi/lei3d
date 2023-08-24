#pragma once

#include <btBulletDynamicsCommon.h>

#include <glm/glm.hpp>

namespace lei3d
{
	inline glm::vec3 btTransformToVec3(btTransform trans)
	{
		return glm::vec3(float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
	}
}