#include "RenderSystem.hpp"

#include "components/ModelInstance.hpp"
#include "components/ColorSource.hpp"
#include "components/SkyBox.hpp"
#include "logging/GLDebug.hpp"

#include "glm/gtc/type_ptr.hpp"
#include <array>

#include "core/AppSettings.hpp"
#include "core/Application.hpp"
#include "gui/GuiManager.hpp"
#include "gui/components/GuiRect.hpp"
#include "gui/components/GuiTextBox.hpp"

namespace lei3d
{

	void RenderSystem::initialize(int width, int height)
	{
		scwidth = width;
		scheight = height;
		shadowResolution = AppSettings::ShadowQuality == 1 ? 4096 : 2048;

		depthShader = Shader("./data/shaders/depth.vert", "./data/shaders/null.frag");
		forwardShader = Shader("./data/shaders/forward.vert", "./data/shaders/forward.frag");
		postprocessShader = Shader("./data/shaders/screenspace_quad.vert", "./data/shaders/postprocess.frag");
		shadowCSMShader = Shader("./data/shaders/shadow_depth.vert", "./data/shaders/null.frag", "./data/shaders/depth_cascades.geom");

		SSRShader = Shader("./data/shaders/screenspace_quad.vert", "./data/shaders/SSR.frag");
		reflectionShader = Shader("./data/shaders/screenspace_quad.vert", "./data/shaders/reflection_blend.frag");

		glGenVertexArrays(1, &dummyVAO);

		// Offscreen render resources

		glGenFramebuffers(1, &FBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);

		glGenTextures(1, &rawTexture);
		glGenTextures(1, &saturationMask);
		glGenTextures(1, &depthTexture);
		glGenTextures(1, &albedoTexture);
		glGenTextures(1, &normalsTexture);
		glGenTextures(1, &metallicRoughnessTexture);
		glGenTextures(1, &reflectionTexture);
		glGenTextures(1, &finalTexture);

		// lighting pass
		GLenum mipMode = AppSettings::SSR ? GL_LINEAR_MIPMAP_NEAREST : GL_NEAREST;
		glBindTexture(GL_TEXTURE_2D, rawTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, rawTexture, 0);

		// saturation buffer
		glBindTexture(GL_TEXTURE_2D, saturationMask);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, width, height, 0, GL_RED, GL_FLOAT,
			nullptr); // single channel for one float values
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, saturationMask, 0);

		// SSR resources

		// screen object albedo
		glBindTexture(GL_TEXTURE_2D, albedoTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, albedoTexture, 0);

		// screen object normals
		glBindTexture(GL_TEXTURE_2D, normalsTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, normalsTexture, 0);

		// screen object metallic + roughness
		glBindTexture(GL_TEXTURE_2D, metallicRoughnessTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, width, height, 0, GL_RG, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, metallicRoughnessTexture, 0);

		// reflection
		glBindTexture(GL_TEXTURE_2D, reflectionTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5, GL_TEXTURE_2D, reflectionTexture, 0);

		// depth map
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT,
			GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

		// post process pass target
		glBindTexture(GL_TEXTURE_2D, finalTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_FLOAT, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT6, GL_TEXTURE_2D, finalTexture, 0);

		// Shadow resources

		glGenFramebuffers(1, &shadowFBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, shadowFBO);
		glGenTextures(1, &shadowDepth);

		// depth map
		glBindTexture(GL_TEXTURE_2D_ARRAY, shadowDepth);
		glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT16, shadowResolution, shadowResolution, 4, 0, GL_DEPTH_COMPONENT,
			GL_UNSIGNED_SHORT, nullptr); ///< should always be 4 levels
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowDepth, 0);

		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

		SSRShader.bind();
		SSRShader.setInt("DepthMap", 0);
		SSRShader.setInt("NormalMap", 1);
		SSRShader.setInt("MetallicRoughnessMap", 2);
		SSRShader.setInt("RawFinalImage", 3);
		SSRShader.setInt("EnvMap", 4);

		reflectionShader.bind();
		reflectionShader.setInt("DepthMap", 0);
		reflectionShader.setInt("AlbedoMap", 1);
		reflectionShader.setInt("MetallicRoughnessMap", 2);
		reflectionShader.setInt("NormalMap", 3);
		reflectionShader.setInt("ReflectedMap", 4);

//		unsigned int lightMatsIdx = glGetUniformBlockIndex(shadowCSMShader.getShaderID(), "LightSpaceMatrices");
//		glUniformBlockBinding(shadowCSMShader.getShaderID(), lightMatsIdx, 1);
//		lightMatsIdx = glGetUniformBlockIndex(forwardShader.getShaderID(), "LightSpaceMatrices");
//		glUniformBlockBinding(forwardShader.getShaderID(), lightMatsIdx, 1);
	}

	void RenderSystem::draw(const Scene& scene, const SceneView& view)
	{
		// clear the blit image
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
		glDrawBuffer(GL_COLOR_ATTACHMENT5);

		glClearColor(0.2f, 0.8f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		Camera& camera = view.ActiveCamera(scene);
		SkyBox* skyBox = nullptr;
		std::vector<ModelInstance*> modelEntities;
		std::vector<ColorSource*> colorSources;
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
			if (auto cs = entity->GetComponent<ColorSource>())
			{
				if (cs->active)
				{
					colorSources.push_back(cs);
				}
			}
		}
		DirectionalLight* dirLight = scene.m_DirectionalLight.get();

		depthPrePass(modelEntities, camera);
		genShadowPass(modelEntities, dirLight, camera);
		lightingPass(modelEntities, colorSources, dirLight, camera);
		if (skyBox)
		{
			environmentPass(*skyBox, colorSources, camera);
		}

		if (AppSettings::SSR)
		{
			indirectLightingPass(*skyBox, camera);
		}
		postprocessPass(camera);
		UiPass();
	}

	void RenderSystem::depthPrePass(const std::vector<ModelInstance*>& objects, Camera& camera)
	{
		depthShader.bind();
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
		glDrawBuffer(GL_NONE);

		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST); // enable drawing to depth mask and depth testing
		glDepthFunc(GL_LESS);
		glClear(GL_DEPTH_BUFFER_BIT);

		glm::mat4 projection = camera.GetProj();
		depthShader.setUniformMat4("projection", projection);
		glm::mat4 view = camera.GetView();
		depthShader.setUniformMat4("view", view);

		for (auto& obj : objects)
		{
			obj->Draw(&depthShader, RenderFlag::None, 0);
		}

		glDepthMask(GL_FALSE);
		glDisable(GL_DEPTH_TEST);
	}

	void RenderSystem::lightingPass(const std::vector<ModelInstance*>& objects, const std::vector<ColorSource*>& colorSrcs, const DirectionalLight* light, Camera& camera)
	{
		forwardShader.bind();

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO);
		std::array<GLenum, 5> drawBuffers{ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
		glDrawBuffers(drawBuffers.size(), drawBuffers.data());

		glEnable(GL_DEPTH_TEST); // enable drawing to depth mask and depth testing
		glDepthFunc(GL_EQUAL);
		glClearColor(0.f, 0.f, 0.f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glm::mat4 projection = camera.GetProj();
		forwardShader.setUniformMat4("projection", projection);
		glm::mat4 view = camera.GetView();
		forwardShader.setUniformMat4("view", view);

		forwardShader.setVec3("camPos", camera.GetPosition());

		forwardShader.setVec3("dirLight.direction", light->direction);
		forwardShader.setVec3("dirLight.color", light->color);
		forwardShader.setFloat("dirLight.intensity", light->intensity);
		forwardShader.setFloat("dirLight.farPlane", camera.GetFarPlane());
		for (int i = 0; i < light->cascadeLevels.size(); i++)
		{
			forwardShader.setFloat(("dirLight.cascadeDistances[" + std::to_string(i) + "]"), light->cascadeLevels[i]);
		}
//		glBindBufferBase(GL_UNIFORM_BUFFER, 1, light->lsmUBO);

		for (int i = 0; i < light->lightSpaceMatrices.size(); i++)
		{
			forwardShader.setUniformMat4(("lightSpaceMatrices[" + std::to_string(i) + "]"), light->lightSpaceMatrices[i]);
		}

		for (int i = 0; i < colorSrcs.size(); i++)
		{
			forwardShader.setVec3("colorSources[" + std::to_string(i) + "].position", colorSrcs[i]->GetPosition());
			forwardShader.setFloat("colorSources[" + std::to_string(i) + "].radius", colorSrcs[i]->GetRadius());
			forwardShader.setFloat("colorSources[" + std::to_string(i) + "].falloff", colorSrcs[i]->GetFalloff());
		}
		forwardShader.setInt("numColorSources", colorSrcs.size());

		forwardShader.setInt("shadowDepth", 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D_ARRAY, shadowDepth);

		for (auto& obj : objects)
		{
			obj->Draw(&forwardShader, RenderFlag::BindImages, 2);
		}

		glDepthFunc(GL_LESS);
		glDisable(GL_DEPTH_TEST);
	}

	void RenderSystem::environmentPass(const SkyBox& skyBox, const std::vector<ColorSource*>& colorSrcs, Camera& camera)
	{
		std::array<GLenum, 2> drawBuffers{ GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		glDrawBuffers(drawBuffers.size(), drawBuffers.data());
		glEnable(GL_DEPTH_TEST);
		GLCall(glDepthFunc(GL_LEQUAL));

		skyBox.skyboxShader.bind();
		glm::mat4 view = glm::mat4(glm::mat3(camera.GetView()));
		skyBox.skyboxShader.setUniformMat4("view", view);
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)scwidth / (float)scheight, camera.GetNearPlane(), camera.GetFarPlane());
		skyBox.skyboxShader.setUniformMat4("projection", projection);
		glm::mat4 model = glm::identity<glm::mat4>();
		skyBox.skyboxShader.setUniformMat4("model", model);
		skyBox.skyboxShader.setInt("skyboxCubemap", 1);

		for (int i = 0; i < colorSrcs.size(); i++)
		{
			skyBox.skyboxShader.setVec3("colorSources[" + std::to_string(i) + "].position", colorSrcs[i]->GetPosition());
			skyBox.skyboxShader.setFloat("colorSources[" + std::to_string(i) + "].radius", colorSrcs[i]->GetRadius());
			skyBox.skyboxShader.setFloat("colorSources[" + std::to_string(i) + "].falloff", colorSrcs[i]->GetFalloff());
		}
		skyBox.skyboxShader.setInt("numColorSources", colorSrcs.size());

		GLCall(glBindVertexArray(skyBox.skyboxVAO));
		GLCall(glActiveTexture(GL_TEXTURE1));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, skyBox.cubeMapTexture));
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		GLCall(glBindVertexArray(0));
		GLCall(glDepthFunc(GL_LESS));
		glDisable(GL_DEPTH_TEST);
	}

	void RenderSystem::indirectLightingPass(const SkyBox& skyBox, Camera& camera)
	{
		// Generate screen space reflections to texture
		SSRShader.bind();

		glDrawBuffer(GL_COLOR_ATTACHMENT5);
		glClear(GL_COLOR_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normalsTexture);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, metallicRoughnessTexture);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, rawTexture);
		glGenerateMipmap(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyBox.cubeMapTexture);

		glm::mat4 trs = glm::translate(glm::mat4(1.0), glm::vec3(0.5, 0.5, 0.5));
		trs = trs * glm::scale(glm::mat4(1.0), glm::vec3(0.5, 0.5, 1.0));
		glm::mat4 screenScale = glm::scale(glm::mat4(1.0), glm::vec3(scwidth, scheight, 1.0));
		glm::mat4 projToPixel = screenScale * trs * camera.GetProj();

		SSRShader.setUniformMat4("projToPixel", projToPixel);
		SSRShader.setVec2("screenSize", {scwidth, scheight});
		glm::mat4 projection = camera.GetProj();
		glm::mat4 projInv = glm::inverse(projection);
		SSRShader.setUniformMat4("invProjection", projInv);
		glm::mat4 view = camera.GetView();
		glm::mat4 viewInv = glm::inverse(view);
		SSRShader.setUniformMat4("view", view);
		SSRShader.setUniformMat4("invView", viewInv);
		SSRShader.setFloat("nearPlane", camera.GetNearPlane());
		SSRShader.setFloat("farPlane", camera.GetFarPlane());

		glBindVertexArray(dummyVAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		// Blend reflections onto current render
		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		reflectionShader.bind();

		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, albedoTexture);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, metallicRoughnessTexture);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, normalsTexture);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, reflectionTexture);

		reflectionShader.setVec2("screenSize", {scwidth, scheight});
		reflectionShader.setVec3("camPos", camera.GetPosition());
		reflectionShader.setUniformMat4("invView", viewInv);
		reflectionShader.setUniformMat4("invProjection", projInv);

		glBindVertexArray(dummyVAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glDisable(GL_BLEND);
	}

	void RenderSystem::postprocessPass(Camera& camera)
	{
		postprocessShader.bind();

		glDrawBuffer(GL_COLOR_ATTACHMENT6);

		// draw a full screen quad, sample from rendered textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, rawTexture);	   // 0
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, saturationMask);  // 1
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, depthTexture);
		postprocessShader.setInt("RawFinalImage", 0);
		postprocessShader.setInt("SaturationMask", 1); // match active texture bindings
		postprocessShader.setInt("DepthMask", 2);

		postprocessShader.setFloat("nearPlane", camera.GetNearPlane());
		postprocessShader.setFloat("farPlane", camera.GetFarPlane());

		glBindVertexArray(dummyVAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
		glReadBuffer(GL_COLOR_ATTACHMENT6);

		// blit to screen
		glBlitFramebuffer(0, 0, scwidth, scheight, 0, 0, scwidth, scheight, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	}

	void RenderSystem::genShadowPass(const std::vector<ModelInstance*>& objects, DirectionalLight* light, Camera& camera)
	{
		shadowCSMShader.bind();

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, shadowFBO);

		glViewport(0, 0, shadowResolution, shadowResolution);
		glCullFace(GL_FRONT);
		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST); // enable drawing to depth mask and depth testing
		glClear(GL_DEPTH_BUFFER_BIT);

		light->lightSpaceMatrices = getLightSpaceMatrices(light, camera);
		for (int i = 0; i < light->lightSpaceMatrices.size(); i++)
		{
			shadowCSMShader.setUniformMat4(("lightSpaceMatrices[" + std::to_string(i) + "]"), light->lightSpaceMatrices[i]);
		}
		// TODO: figure out why UBOs not working later
	//		glBindBuffer(GL_UNIFORM_BUFFER, light->lsmUBO);
	//		for (int i = 0; i < light->lightSpaceMatrices.size(); i++) {
	//			printf("%llu %llu\n", i * sizeof(glm::mat4x4), sizeof(glm::mat4x4));
	//			GLCall(glBufferSubData(GL_UNIFORM_BUFFER, i * sizeof(glm::mat4x4), sizeof(glm::mat4x4), glm::value_ptr(light->lightSpaceMatrices[i])));
	//		}
	//		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		for (auto& obj : objects)
		{
			obj->Draw(&shadowCSMShader, RenderFlag::None, 0);
		}

		glCullFace(GL_BACK);
		glViewport(0, 0, scwidth, scheight);
	}

	void RenderSystem::UiPass()
	{
		//GuiTextBox* rect = new GuiTextBox();
		//GuiManager::Instance().AddGuiComponent((GuiComponent*)rect);
		//GuiManager::Instance().RenderGui(glm::vec2(scwidth, scheight));
		//delete rect;
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

	glm::mat4 RenderSystem::getLightSpaceMatrix(DirectionalLight* light, float nearPlane, float farPlane, Camera& camera)
	{
		const glm::mat4 projection = glm::perspective(glm::radians(camera.GetFOV()), (float)scwidth / (float)scheight, nearPlane, farPlane);
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

		const glm::mat4 lightProj = glm::ortho(pminX, pmaxX, pminY, pmaxY, pminZ, pmaxZ);
		return lightProj * lightView;
	}
	
	std::vector<glm::mat4> RenderSystem::getLightSpaceMatrices(DirectionalLight* light, Camera& camera)
	{
		std::vector<glm::mat4> matrices;
		matrices.reserve(light->cascadeLevels.size() + 1);
		for (int i = 0; i < light->cascadeLevels.size() + 1; i++)
		{
			if (i == 0)
			{
				matrices.push_back(getLightSpaceMatrix(light, camera.GetNearPlane(), light->cascadeLevels[i], camera));
			}
			else if (i < light->cascadeLevels.size())
			{
				matrices.push_back(getLightSpaceMatrix(light, light->cascadeLevels[i - 1], light->cascadeLevels[i], camera));
			}
			else
			{
				matrices.push_back(getLightSpaceMatrix(light, light->cascadeLevels[i - 1], camera.GetFarPlane(), camera));
			}
		}
		return matrices;
	}

} // namespace lei3d