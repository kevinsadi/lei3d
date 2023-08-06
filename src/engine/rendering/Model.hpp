#pragma once

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#ifndef STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

#include "core/Component.hpp"
#include "rendering/Shader.hpp"
#include "rendering/Mesh.hpp"
#include "logging/Log.hpp";

namespace lei3d
{
    class Component;

    class Model
    {
    public:
        std::vector<Texture> textures_loaded;
        Shader* m_Shader;    //This is temporary. We want to abstract the shader system eventually to handle them better.
        
        Model(const std::string& modelPath);
        ~Model();

        void Draw(Shader& shader);

        std::vector<btTriangleMesh*> GetCollisionMeshes() const;
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