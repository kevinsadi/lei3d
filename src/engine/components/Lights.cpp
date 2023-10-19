#include "Lights.hpp"

#include "core/Application.hpp"
#include "logging/GLDebug.hpp"

namespace lei3d
{
	DirectionalLight::DirectionalLight(glm::vec3 dir, glm::vec3 col, float intensity)
		: direction(glm::normalize(dir)), color(col), intensity(intensity)
	{
		Camera& camera = Application::GetInstance().GetSceneCamera();
		float farZ = camera.GetFarPlane();
		cascadeLevels = std::vector<float>{ farZ * 0.067f, farZ * 0.2f, farZ * 0.5f };

//		GLCall(glGenBuffers(1, &lsmUBO));
//		GLCall(glBindBuffer(GL_UNIFORM_BUFFER, lsmUBO));
//		GLCall(glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4x4) * 4, nullptr, GL_STATIC_DRAW));
//		GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, 1, lsmUBO));
//		GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
	}
} // namespace lei3d