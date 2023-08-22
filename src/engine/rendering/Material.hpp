#pragma once

#include <glm/glm.hpp>
#include <rendering/Shader.hpp>

namespace lei3d {

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

class Material {
public:
	Material() {}

	void bind(Shader& shader, unsigned int tex_offset);
	void unbind(unsigned int tex_offset);

	glm::vec3 m_Albedo{ 0.8f, 0.8f, 0.8f };
	float m_Metallic = 0.f;
	float m_Roughness = 0.5f;
	float m_Ambient = 1.f;

	std::shared_ptr<Texture> m_AlbedoTexture = nullptr;
	std::shared_ptr<Texture> m_MetallicTexture = nullptr;
	std::shared_ptr<Texture> m_RoughnessTexture = nullptr;
	std::shared_ptr<Texture> m_AmbientTexture = nullptr;

	std::shared_ptr<Texture> m_NormalMap = nullptr;
	std::shared_ptr<Texture> m_BumpMap = nullptr;

	bool m_UseAlbedoMap = false;
	bool m_UseMetallicMap = false;
	bool m_UseRoughnessMap = false;
	bool m_IsGlossyRough = false;
	bool m_UseAmbientMap = false;
	bool m_UseNormalMap = false;
	bool m_UseBumpMap = false;
	float m_BumpScale = 0.1f;

private:
	int texture_count = 0;
};

}  // namespace lei3d
