#include "ColorSource.hpp"

#include "logging/LogGLM.hpp"
#include <array>

namespace lei3d
{
	ColorSource::ColorSource(Entity& entity)
		: Component(entity)
	{
	}

	ColorSource::~ColorSource()
	{
	}

	void ColorSource::Init(float r, float f, bool a)
	{
		radius = r;
		falloff = f;
		active = a;
	}

	void ColorSource::ToggleActive()
	{
		active = !active;
	}

	void ColorSource::SetActive(bool a)
	{
		active = a;
	}

	float ColorSource::GetRadius()
	{
		return radius;
	}

	glm::vec3 ColorSource::GetPosition()
	{
		return m_Entity.m_Transform.position;
	}

	float ColorSource::GetFalloff()
	{
		return falloff;
	}
} // namespace lei3d