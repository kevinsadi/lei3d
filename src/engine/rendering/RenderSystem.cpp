#include "RenderSystem.hpp"

#include "components/ModelInstance.hpp"
#include "components/SkyBox.hpp"
#include "logging/GLDebug.hpp"

#include <array>

namespace lei3d
{

	void RenderSystem::initialize(int width, int height)
	{
		scwidth = width;
		scheight = height;

		forwardShader = Shader("./data/shaders/forward.vert", "./data/shaders/forward.frag");
		postprocessShader = Shader("./data/shaders/screenspace_quad.vert", "./data/shaders/postprocess.frag");
		shadowEVSMShader = Shader("./data/shaders/shadow_depth.vert", "./data/shaders/shadowEVSM.frag");

		glGenVertexArrays(1, &dummyVAO);

		// Offscreen render resources

		glGenFramebuffers(1, &FBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);

		glGenTextures(1, &rawTexture);
		glGenTextures(1, &saturationMask);
		glGenTextures(1, &depthStencilTexture);
		glGenTextures(1, &finalTexture);

		// lighting pass
		glBindTexture(GL_TEXTURE_2D, rawTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rawTexture, 0);

		// saturation buffer
		glBindTexture(GL_TEXTURE_2D, saturationMask);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT,
			nullptr); // single channel for one float values
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, saturationMask, 0);

		// depth map
		glBindTexture(GL_TEXTURE_2D, depthStencilTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, width, height, 0, GL_DEPTH_STENCIL,
			GL_FLOAT_32_UNSIGNED_INT_24_8_REV, nullptr); // probably won't need stencil
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthStencilTexture, 0);

		// post process pass target
		glBindTexture(GL_TEXTURE_2D, finalTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, finalTexture, 0);

		// Shadow resources

		glGenFramebuffers(1, &shadowFBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, shadowFBO);

		glGenTextures(1, &shadowMoments);
		glGenTextures(1, &shadowDepth);

		// shadow moments map
		glBindTexture(GL_TEXTURE_2D, shadowMoments);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, shadowResolution, shadowResolution, 0, GL_RGBA, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, shadowMoments, 0);

		// depth map; unused
		glBindTexture(GL_TEXTURE_2D, shadowDepth);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, shadowResolution, shadowResolution, 0, GL_DEPTH_COMPONENT,
			GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowDepth, 0);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		///< error checks
	}

	void RenderSystem::draw(const Scene& scene)
	{
		// clear the blit image
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
		glDrawBuffer(GL_COLOR_ATTACHMENT2);

		glClearColor(0.2f, 0.8f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		FlyCamera&					camera = scene.MainCamera();
		SkyBox*						skyBox = nullptr;
		std::vector<ModelInstance*> modelEntities;
		for (auto& entity : scene.m_Entities)
		{
			if (auto mi = entity->GetComponent<ModelInstance>())
			{
				modelEntities.push_back(mi);
			}
			if (auto sb = entity->GetComponent<SkyBox>())
			{
				skyBox = sb;
			}
		}
		DirectionalLight* dirLight = scene.m_DirectionalLight.get();

		genShadowPass(modelEntities, dirLight, camera);
		lightingPass(modelEntities, dirLight, camera);
		if (skyBox)
		{
			environmentPass(*skyBox, camera);
		}
		postprocessPass();
	}

	void RenderSystem::lightingPass(const std::vector<ModelInstance*>& objects, const DirectionalLight* light, FlyCamera& camera)
	{
		forwardShader.bind();

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
		std::array<GLenum, 2> drawBuffers{ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(drawBuffers.size(), drawBuffers.data()); // set attachment targets as 0 and 1

		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST); // enable drawing to depth mask and depth testing
		glClearColor(0.f, 0.f, 0.f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = camera.GetProj();
		forwardShader.setUniformMat4("projection", projection);
		glm::mat4 view = camera.GetView();
		forwardShader.setUniformMat4("view", view);

		forwardShader.setVec3("camPos", camera.GetPosition());

		forwardShader.setVec3("dirLight.direction", light->direction);
		forwardShader.setVec3("dirLight.color", light->color);
		forwardShader.setFloat("dirLight.intensity", light->intensity);
//		forwardShader.setFloat("dirLight.nearPlane", light->nearPlane);
//		forwardShader.setFloat("dirLight.farPlane", light->farPlane);
		forwardShader.setUniformMat4("lightSpaceMatrix", lightSpaceMat);

		forwardShader.setInt("shadowMoments", 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, shadowDepth);

		for (auto& obj : objects)
		{
			obj->Draw(&forwardShader, RenderFlag::BindImages, 2);
		}

		glDepthMask(GL_FALSE);
		glDisable(GL_DEPTH_TEST);
	}

	void RenderSystem::environmentPass(const SkyBox& skyBox, FlyCamera& camera)
	{
		glEnable(GL_DEPTH_TEST);
		GLCall(glDepthFunc(GL_LEQUAL)); // we change the depth function here to it passes when testing depth value is equal
										// to what is current stored
		skyBox.skyboxShader.bind();
		glm::mat4 view = glm::mat4(glm::mat3(camera.GetView()));
		skyBox.skyboxShader.setUniformMat4("view", view);
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)scwidth / (float)scheight, 0.1f, 400.0f);
		skyBox.skyboxShader.setUniformMat4("projection", projection);
		glm::mat4 model = glm::identity<glm::mat4>();
		skyBox.skyboxShader.setUniformMat4("model", model);
		skyBox.skyboxShader.setInt("skyboxCubemap", 0);
		// -- render the skybox cube
		GLCall(glBindVertexArray(skyBox.skyboxVAO));
		GLCall(glActiveTexture(GL_TEXTURE0)); //! could be the problem
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, skyBox.cubeMapTexture));
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		GLCall(glBindVertexArray(0));
		GLCall(glDepthFunc(GL_LESS)); // set depth function back to normal
		glDisable(GL_DEPTH_TEST);
	}

	void RenderSystem::postprocessPass()
	{
		postprocessShader.bind();

		glDrawBuffer(GL_COLOR_ATTACHMENT2);

		// draw a full screen quad, sample from rendered textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, rawTexture);	   // 0
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, saturationMask);  // 1
		postprocessShader.setInt("RawFinalImage", 0);
		postprocessShader.setInt("SaturationMask", 1); // match active texture bindings

		glBindVertexArray(dummyVAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
		glReadBuffer(GL_COLOR_ATTACHMENT2);

		// blit to screen
		glBlitFramebuffer(0, 0, scwidth, scheight, 0, 0, scwidth, scheight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	}

	void RenderSystem::genShadowPass(const std::vector<ModelInstance*>& objects, DirectionalLight* light, FlyCamera& camera)
	{
		shadowEVSMShader.bind();

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, shadowFBO);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		glViewport(0, 0, shadowResolution, shadowResolution);
		glCullFace(GL_FRONT);
		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST); // enable drawing to depth mask and depth testing
		glClearColor(0.f, 0.f, 0.f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightSpaceMat = getLightSpaceMatrix(light, 0.1f, 500.f, camera);	// TODO: set it to camera planes?
		shadowEVSMShader.setUniformMat4("lightSpaceMatrix", lightSpaceMat);
		shadowEVSMShader.setVec3("lightPos", 150.f * -light->direction);
		shadowEVSMShader.setFloat("lightNearPlane", light->nearPlane);
		shadowEVSMShader.setFloat("lightFarPlane", light->farPlane);

		for (auto& obj : objects)
		{
			obj->Draw(&shadowEVSMShader, RenderFlag::None, 0);
		}

		glCullFace(GL_BACK);
		glViewport(0, 0, scwidth, scheight);
	}

	std::vector<glm::vec4> RenderSystem::getFrustumCornersWS(const glm::mat4& projection, const glm::mat4& view)
	{
		glm::mat4 invVP = glm::inverse(projection * view);
		std::vector<glm::vec4> corners;
		for (int x = 0; x < 2; x++)
		{
			for (int y = 0; y < 2; y++)
			{
				for (int z = 0; z < 2; z++)
				{
					glm::vec4 c = invVP * glm::vec4{ 2.0f * x - 1.0f, 2.0f * y - 1.0f, 2.0f * z - 1.0f, 1.0f };
					corners.push_back(c / c.w);
				}
			}
		}
		return corners;
	}

	glm::mat4 RenderSystem::getLightSpaceMatrix(DirectionalLight* light, float nearPlane, float farPlane, FlyCamera& camera)
	{
		const glm::mat4	projection = glm::perspective(glm::radians(camera.GetFOV()), (float)scwidth / (float)scheight, nearPlane, farPlane);
		const std::vector<glm::vec4> corners = getFrustumCornersWS(projection, camera.GetView());

		glm::vec3 center = glm::vec3(0.f);
		for (const auto& c : corners)
		{
			center += glm::vec3(c);
		}
		center /= corners.size();

		const glm::mat4 lightView = glm::lookAt(center - light->direction, center, glm::vec3{ 0, 1, 0 });
		float pminX = std::numeric_limits<float>::max();
		float pmaxX = std::numeric_limits<float>::lowest();
		float pminY = std::numeric_limits<float>::max();
		float pmaxY = std::numeric_limits<float>::lowest();
		float pminZ = std::numeric_limits<float>::max();
		float pmaxZ = std::numeric_limits<float>::lowest();
		for (const auto& c : corners)
		{
			const auto trf = lightView * c;
			pminX = std::min(pminX, trf.x);
			pmaxX = std::max(pmaxX, trf.x);
			pminY = std::min(pminY, trf.y);
			pmaxY = std::max(pmaxY, trf.y);
			pminZ = std::min(pminZ, trf.z);
			pmaxZ = std::max(pmaxZ, trf.z);
		}

		float frustum_fitting_factor = 1.f;
		if (pminZ < 0)
		{
			pminZ *= frustum_fitting_factor;
		}
		else
		{
			pminZ /= frustum_fitting_factor;
		}
		if (pmaxZ < 0)
		{
			pmaxZ /= frustum_fitting_factor;
		}
		else
		{
			pmaxZ *= frustum_fitting_factor;
		}

		light->nearPlane = pminZ;
		light->farPlane = pmaxZ;
		const glm::mat4 lightProj = glm::ortho(pminX, pmaxX, pminY, pmaxY, pminZ, pmaxZ);
		return lightProj * lightView;
	}

} // namespace lei3d