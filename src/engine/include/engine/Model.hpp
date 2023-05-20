#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#ifndef STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

#include "include/engine/Shader.hpp"
#include "include/engine/Mesh.hpp"

namespace kek3d
{
    class Model
    {
    public:
        std::vector<Texture> texturesLoaded;
        
        Model(char *path);
        void Draw(Shader &shader);
    private:
        // model data
        std::vector<Mesh> meshes;
        std::string directory;

        void loadModel(std::string path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
    };

    unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);
}