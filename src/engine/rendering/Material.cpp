#include "Material.hpp"

namespace lei3d {

    void Material::bind(Shader &shader, unsigned int tex_offset) {
        int curr_offset = 0;
        if (!use_albedo_map) {
            shader.setVec3("material.albedo", albedo);
        } else {
            shader.setInt("material.texture_albedo", tex_offset + curr_offset);
            glActiveTexture(GL_TEXTURE0 + tex_offset + curr_offset);
            glBindTexture(GL_TEXTURE_2D, albedo_texture->id);
            curr_offset++;
        }
        if (!use_metallic_map) {
            shader.setFloat("material.metallic", metallic);
        } else {
            shader.setInt("material.texture_metallic", tex_offset + curr_offset);
            glActiveTexture(GL_TEXTURE0 + tex_offset + curr_offset);
            glBindTexture(GL_TEXTURE_2D, metallic_texture->id);
            curr_offset++;
        }
        if (!use_roughness_map) {
            shader.setFloat("material.roughness", roughness);
        } else {
            shader.setInt("material.texture_roughness", tex_offset + curr_offset);
            glActiveTexture(GL_TEXTURE0 + tex_offset + curr_offset);
            glBindTexture(GL_TEXTURE_2D, roughness_texture->id);
            curr_offset++;
        }
        if (!use_ao_map) {
            shader.setFloat("material.ambient", ambient);
        } else {
            shader.setInt("material.texture_ao", tex_offset + curr_offset);
            glActiveTexture(GL_TEXTURE0 + tex_offset + curr_offset);
            glBindTexture(GL_TEXTURE_2D, ao_texture->id);
            curr_offset++;
        }

        if (use_normal_map) {
            shader.setInt("material.texture_normal", tex_offset + curr_offset);
            glActiveTexture(GL_TEXTURE0 + tex_offset + curr_offset);
            glBindTexture(GL_TEXTURE_2D, normal_map->id);
            curr_offset++;
        }
        if (use_bump_map) {
            shader.setInt("material.texture_bump", tex_offset + curr_offset);
            glActiveTexture(GL_TEXTURE0 + tex_offset + curr_offset);
            glBindTexture(GL_TEXTURE_2D, bump_map->id);
            curr_offset++;
        }
        glActiveTexture(GL_TEXTURE0);
        texture_count = curr_offset;

        ///< if someone knows how to add shader defines instead, pls replace
        shader.setBool("material.use_albedo_map", use_albedo_map);
        shader.setBool("material.use_metallic_map", use_metallic_map);
        shader.setBool("material.use_roughness_map", use_roughness_map);
        shader.setBool("material.use_ao_map", use_ao_map);

        shader.setBool("material.use_normal_map", use_normal_map);
        shader.setBool("material.use_bump_map", use_bump_map);
    }

    void Material::unbind(unsigned int tex_offset) {
        for (unsigned int i = 0; i < texture_count; i++) {
            glActiveTexture(GL_TEXTURE0 + (i + tex_offset));
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

} // lei3d