#ifndef MODEL
#define MODEL

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#ifndef STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

#include "engine/Shader.hpp"
#include "engine/Mesh.hpp"
#include "engine/Material.hpp"
#include "util/Log.hpp"

namespace lei3d
{
    class Model
    {
    public:
        std::vector<std::shared_ptr<Texture>> textures;
        std::vector<std::shared_ptr<Material>> materials;

        Model();
        Model(const std::string& path);
        ~Model();

        void Draw(Shader &shader);
    private:
        friend Model;

        // model data
        std::vector<Mesh> meshes;
        std::string directory;

        void loadMaterials(const aiScene* scene);
        void loadModel(const std::string& path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        std::shared_ptr<Texture> loadMaterialTexture(const aiMaterial *mat, aiTextureType type, const std::string& typeName);
    };

    unsigned int TextureFromFile(const char *path, const std::string& directory, bool gamma = false);
}

#endif // MODEL