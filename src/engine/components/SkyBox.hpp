#pragma once

#include <glad/glad.h>

#include <iostream>
#include <string>
#include <vector>

#ifndef STB_IMAGE_IMPLEMENTATION
	#include <stb_image.h>
#endif

#include "core/Component.hpp"
#include "rendering/Shader.hpp"

namespace lei3d
{

	class SkyBox : public Component
	{
	public:
		unsigned int skyboxVAO;
		unsigned int skyboxVBO;
		Shader		 skyboxShader;
		unsigned int cubeMapTexture;

		SkyBox(Entity& entity);
		~SkyBox();

		// std::string GetComponentName() override;
		void Init(const std::vector<std::string>& faces);

		void Render() override;

	private:
		void loadCubemap(const std::vector<std::string>& faces);
	};
} // namespace lei3d