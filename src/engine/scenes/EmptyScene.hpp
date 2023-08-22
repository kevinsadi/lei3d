#pragma once

#include "components/SkyBox.hpp"
#include "core/Entity.hpp"
#include "core/Scene.hpp"

namespace lei3d
{

	class Entity;
	class Model;
	class Shader;
	class SkyBox;

	class EmptyScene : public Scene
	{
	public:
		EmptyScene();
		~EmptyScene();

		void OnLoad() override;
	};

} // namespace lei3d