#include "Material.hpp"

namespace lei3d {

void Material::bind(Shader &shader, unsigned int tex_offset) {
	int curr_offset = 0;
	if (!m_UseAlbedoMap) {
		shader.setVec3("material.albedo", m_Albedo);
	} else {
		shader.setInt("material.texture_albedo", tex_offset + curr_offset);
		glActiveTexture(GL_TEXTURE0 + tex_offset + curr_offset);
		glBindTexture(GL_TEXTURE_2D, m_AlbedoTexture->id);
		curr_offset++;
	}
	if (!m_UseMetallicMap) {
		shader.setFloat("material.metallic", m_Metallic);
	} else {
		shader.setInt("material.texture_metallic", tex_offset + curr_offset);
		glActiveTexture(GL_TEXTURE0 + tex_offset + curr_offset);
		glBindTexture(GL_TEXTURE_2D, m_MetallicTexture->id);
		curr_offset++;
	}
	if (!m_UseRoughnessMap) {
		shader.setFloat("material.roughness", m_Roughness);
	} else {
		shader.setInt("material.texture_roughness", tex_offset + curr_offset);
		glActiveTexture(GL_TEXTURE0 + tex_offset + curr_offset);
		glBindTexture(GL_TEXTURE_2D, m_RoughnessTexture->id);
		curr_offset++;
	}
	if (!m_UseAmbientMap) {
		shader.setFloat("material.ambient", m_Ambient);
	} else {
		shader.setInt("material.texture_ao", tex_offset + curr_offset);
		glActiveTexture(GL_TEXTURE0 + tex_offset + curr_offset);
		glBindTexture(GL_TEXTURE_2D, m_AmbientTexture->id);
		curr_offset++;
	}

	if (m_UseNormalMap) {
		shader.setInt("material.texture_normal", tex_offset + curr_offset);
		glActiveTexture(GL_TEXTURE0 + tex_offset + curr_offset);
		glBindTexture(GL_TEXTURE_2D, m_NormalMap->id);
		curr_offset++;
	}
	if (m_UseBumpMap) {
		shader.setInt("material.texture_bump", tex_offset + curr_offset);
		glActiveTexture(GL_TEXTURE0 + tex_offset + curr_offset);
		glBindTexture(GL_TEXTURE_2D, m_BumpMap->id);
		curr_offset++;
	}
	glActiveTexture(GL_TEXTURE0);
	texture_count = curr_offset;

	///< if someone knows how to add shader defines instead, pls replace
	shader.setBool("material.use_albedo_map", m_UseAlbedoMap);
	shader.setBool("material.use_metallic_map", m_UseMetallicMap);
	shader.setBool("material.use_roughness_map", m_UseRoughnessMap);
	shader.setBool("material.use_ao_map", m_UseAmbientMap);

	shader.setBool("material.use_normal_map", m_UseNormalMap);
	shader.setBool("material.use_bump_map", m_UseBumpMap);
}

void Material::unbind(unsigned int tex_offset) {
	for (unsigned int i = 0; i < texture_count; i++) {
		glActiveTexture(GL_TEXTURE0 + (i + tex_offset));
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

}  // namespace lei3d