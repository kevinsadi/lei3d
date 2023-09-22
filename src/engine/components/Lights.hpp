#pragma once

#include "glm/glm.hpp"
#include <vector>

namespace lei3d
{

	class DirectionalLight
	{
	public:
		DirectionalLight(glm::vec3 dir = glm::vec3{ 0, -1, 0 }, glm::vec3 col = glm::vec3{ 1.f }, float intensity = 1.f);

		glm::vec3 direction;
		glm::vec3 color;
		float intensity;

		std::vector<float> cascadeLevels;
		std::vector<glm::mat4> lightSpaceMatrices;
//		unsigned int lsmUBO;
	};

} // namespace lei3d
