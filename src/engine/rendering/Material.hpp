#pragma once

#include <glm/glm.hpp>
#include <rendering/Shader.hpp>
#include <memory>

namespace lei3d
{

	struct Texture
	{
		unsigned int id;
		std::string	 type;
		std::string	 path;
	};

	class Material
	{
	public:
		Material() {}

		void bind(Shader& shader, unsigned int tex_offset);
		void unbind(unsigned int tex_offset);

		glm::vec3 m_Albedo{ 0.8f, 0.8f, 0.8f };
		float	  m_Metallic = 0.f;
		float	  m_Roughness = 1.f;
		float	  m_Ambient = 1.f;

		Texture* m_AlbedoTexture = nullptr;
		Texture* m_MetallicTexture = nullptr;
		Texture* m_RoughnessTexture = nullptr;
		Texture* m_AmbientTexture = nullptr;

		Texture* m_NormalMap = nullptr;
		Texture* m_BumpMap = nullptr;

		bool  m_UseAlbedoMap = false;
		bool  m_UseMetallicMap = false;
		bool  m_UseRoughnessMap = false;
		bool  m_IsGlossyRough = false;
		bool  m_UseAmbientMap = false;
		bool  m_UseNormalMap = false;
		bool  m_UseBumpMap = false;
		float m_BumpScale = 0.1f;

	private:
		int texture_count = 0;
	};

} // namespace lei3d
