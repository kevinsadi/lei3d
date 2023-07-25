#pragma once

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#ifndef STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

#include "engine/Component.hpp"
#include "engine/Shader.hpp"
#include "engine/Mesh.hpp"
#include "util/Log.hpp";

namespace lei3d
{
    class Component;

    class Model : public Component
    {
    public:
        std::vector<Texture> textures_loaded;
        Shader* prop_shader;
        
        Model(Entity* entity);
        ~Model();
        
        //std::string GetComponentName() override;
        void Init(const std::string& modelPath, Shader& shader);

        void Render() override;
    private:
        // model data
        std::vector<Mesh> meshes;
        std::string directory;

        void loadModel(const std::string& path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::string& typeName);
    };

    unsigned int TextureFromFile(const char *path, const std::string& directory, bool gamma = false);
}