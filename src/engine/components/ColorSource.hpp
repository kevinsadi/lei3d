#pragma once

#include "core/Component.hpp"
#include "logging/Log.hpp"

#include <string>
#include <vector>

namespace lei3d
{

	class ColorSource : public Component
	{
	public:
		float radius;
		bool active;

		ColorSource(Entity& entity);
		~ColorSource();

		void Init(float radius, bool active = false);

		void ToggleActive();
		void SetActive(bool active);

		float GetRadius();
		glm::vec3 GetPosition();
	};

} // namespace lei3d
