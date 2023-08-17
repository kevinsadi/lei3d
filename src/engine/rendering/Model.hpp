#pragma once

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#ifndef STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

#include "rendering/Shader.hpp"
#include "rendering/Mesh.hpp"
#include "logging/Log.hpp"
#include "BulletCollision/CollisionShapes/btTriangleMesh.h"

namespace lei3d
{
    class Component;

    class Model
    {
    private:
        // model data
        std::vector<Mesh> m_Meshes;
        std::string m_Directory;
        std::vector<Texture> m_TexturesLoaded;
    public:
        std::vector<std::shared_ptr<Texture>> textures;
        std::vector<std::shared_ptr<Material>> materials;

        Model(const std::string& modelPath);
        ~Model();

        void Draw(Shader& shader);

        std::vector<btTriangleMesh*> GetCollisionMeshes() const;
    private:

        void loadMaterials(const aiScene* scene);
        void loadModel(const std::string& path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        std::shared_ptr<Texture> loadMaterialTexture(const aiMaterial *mat, aiTextureType type, const std::string& typeName);
    };

    //TODO: Use Abstract Texture in "Texture.cpp"
    unsigned int TextureFromFile(const char *path, const std::string& directory, bool gamma = false);
}