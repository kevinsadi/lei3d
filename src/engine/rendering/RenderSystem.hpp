#pragma once

#include "core/Component.hpp"
#include "core/Scene.hpp"

#include "components/Camera.hpp"

#include "rendering/Shader.hpp"

namespace lei3d
{

	class ModelInstance;
	class SkyBox;

	class RenderSystem
	{
	public:
		RenderSystem()
		{
		}

		~RenderSystem()
		{
		}

		void initialize(int width, int height);

		void draw(const Scene& scene);

	private:
		void lightingPass(const std::vector<ModelInstance*>& objects, Camera& camera);

		void environmentPass(const SkyBox& skyBox, Camera& camera);

		void postprocessPass();

		// offscreen render target objects
		unsigned int FBO;
		unsigned int rawTexture;
		unsigned int saturationMask;
		unsigned int depthStencilTexture;
		unsigned int finalTexture;

		unsigned int dummyVAO; // used to draw full-screen "quad"

		int scwidth, scheight;

		// shaders
		Shader forwardShader;
		Shader postprocessShader;
	};

} // namespace lei3d
