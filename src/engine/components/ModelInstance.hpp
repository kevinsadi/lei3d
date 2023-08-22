#pragma once

#include "core/Component.hpp"
#include "logging/Log.hpp"
#include "rendering/Model.hpp"
#include "rendering/Shader.hpp"

#include <string>
#include <vector>

namespace lei3d
{
	class ModelInstance : public Component
	{
	public:
		Model* m_Model;

		ModelInstance(Entity& entity);
		~ModelInstance();

		void Init(Model* model);

		void Draw(Shader* shader);

	private:
	};

} // namespace lei3d