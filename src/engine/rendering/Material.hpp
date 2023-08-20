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

        glm::vec3 albedo{0.8f, 0.8f, 0.8f};
        float metallic = 0.f;
        float roughness = 0.5f;
        float ambient = 1.f;

        std::shared_ptr<Texture> albedo_texture = nullptr;
        std::shared_ptr<Texture> metallic_texture = nullptr;
        std::shared_ptr<Texture> roughness_texture = nullptr;
        std::shared_ptr<Texture> ao_texture = nullptr;

        std::shared_ptr<Texture> normal_map = nullptr;
        std::shared_ptr<Texture> bump_map = nullptr;

        bool use_albedo_map = false;
        bool use_metallic_map = false;
        bool use_roughness_map = false;
        bool is_glossy_rough = false;
        bool use_ao_map = false;
        bool use_normal_map = false;
        bool use_bump_map = false;
        float bump_scale = 0.1f;

    private:
        int texture_count = 0;
    };

} // lei3d
