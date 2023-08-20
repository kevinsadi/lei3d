#include "Model.hpp"

#include "logging/LogGLM.hpp"

namespace lei3d
{
    Model::Model(const std::string& modelPath)
    {     
        loadModel(modelPath); 
    }

    Model::~Model()
    {
    }

    void Model::Draw(Shader& shader)
    {
        for (unsigned int i = 0; i < this->m_Meshes.size(); i++)
        {
            m_Meshes[i].Draw(shader);
        }
    }

    void Model::loadModel(const std::string& path)
    {
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_CalcTangentSpace | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::string errorString = importer.GetErrorString();
            LEI_WARN("ERROR::ASSIMP::" + errorString);
            return;
        }
        m_Directory = path.substr(0, path.find_last_of('/'));

        loadMaterials(scene);
        processNode(scene->mRootNode, scene);
    }

    void Model::processNode(aiNode* node, const aiScene* scene)
    {
        // process node's meshes
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            m_Meshes.push_back(processMesh(mesh, scene));
        }

        // process node children
        for (unsigned int i = 0; i < node ->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

    /**
    * Given an mesh and the scene it belongs to, create our own representation of a mesh
    * object that we can render.
    * 
    */
    Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        // process vertices from assimp to our own mesh component
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex{};

            glm::vec3 pos;
            pos.x = mesh->mVertices[i].x;
            pos.y = mesh->mVertices[i].y;
            pos.z = mesh->mVertices[i].z;
            vertex.Position = pos;

            if (mesh->HasNormals())
            {
                glm::vec3 normal;
                normal.x = mesh->mNormals[i].x;
                normal.y = mesh->mNormals[i].y;
                normal.z = mesh->mNormals[i].z;
                vertex.Normal = normal; 
            }

            if(mesh->mTextureCoords[0]) // are there any uv coordinates associated with mesh
            {
                glm::vec2 uv;
                uv.x = mesh->mTextureCoords[0][i].x; 
                uv.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = uv;
            }
            else
            {
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);  
            }

            if (mesh->mTangents) {
                glm::vec3 tangent(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
                vertex.Tangent = tangent;
            }

            vertices.push_back(vertex);
        }

        // we set all of our faces to be triangles, so it's easy to get the indices we need
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++)
            {
                indices.push_back(face.mIndices[j]);
            }
        }

        // finally load all the materials we want
        std::shared_ptr<Material> material = nullptr;
        if(mesh->mMaterialIndex >= 0)
        {
            material = materials[mesh->mMaterialIndex];
        }

        // return a mesh object created from the extracted mesh data
        return {vertices, indices, material};
    }

    // get the materials that we want from the assimp mat and convert it to textures array that is returned
    std::shared_ptr<Texture> Model::loadMaterialTexture(const aiMaterial *mat, aiTextureType type, const std::string& typeName)
    {
        std::shared_ptr<Texture> texture;

        if (mat->GetTextureCount(type) < 1) {
            // no textures of this type
            return nullptr;
        }
        if (mat->GetTextureCount(type) > 1) {
            std::cout << "Found more than 1 texture for type " << aiTextureTypeToString(type) << " in material " << mat->GetName().C_Str() << "\n";
        }

        // only select the first texture
        aiString str;
        mat->GetTexture(type, 0, &str);

        bool skip = false;
        for (const auto& tex : textures) // for every texture we've loaded so far
        {
            if (std::strcmp(str.C_Str(), tex->path.c_str()) == 0)
            {
                skip = true;
            }
        }

        if (!skip)
        {
            texture = std::make_shared<Texture>();
            texture->id = TextureFromFile(str.C_Str(), m_Directory);
            texture->type = typeName;
            texture->path = str.C_Str();
            textures.push_back(texture);
        }

        return texture;
    }

    /**
     * @brief Creates collision meshes From Model object
     * 
     * Requires the Model to have loaded it's meshes. Leverages the vertices and indices of each mesh to create a
     * btTriangleMesh for each Mesh.
     * 
     * @return std::vector<btTriangleMesh> 
     */
    std::vector<btTriangleMesh*> Model::GetCollisionMeshes() const
    {
        std::vector<btTriangleMesh*> collisionMeshList;

        for (const Mesh& mesh : m_Meshes)
        {
            btTriangleMesh* curCollisionMesh = new btTriangleMesh();

            std::vector<Vertex> vertices = mesh.vertices;
            std::vector<unsigned int> indices = mesh.indices;

            for (int i = 0; i < indices.size(); i += 3)
            {
                glm::vec3 vert1 = vertices[indices[i]].Position;
                btVector3 bvert1(vert1.x, vert1.y, vert1.z);
                glm::vec3 vert2 = vertices[indices[i+1]].Position;
                btVector3 bvert2(vert2.x, vert2.y, vert2.z);
                glm::vec3 vert3 = vertices[indices[i+2]].Position;
                btVector3 bvert3(vert3.x, vert3.y, vert3.z);

                curCollisionMesh->addTriangle(bvert1, bvert2, bvert3);
            }
            collisionMeshList.push_back(curCollisionMesh);
        }

        return collisionMeshList;
    }

    void Model::loadMaterials(const aiScene *scene) {
        for (size_t i = 0; i < scene->mNumMaterials; i++) {
            const aiMaterial* aimaterial = scene->mMaterials[i];

            std::shared_ptr<Material> newMaterial = std::make_shared<Material>();

            aiColor3D color;
            if (aimaterial->Get(AI_MATKEY_BASE_COLOR, color) == AI_SUCCESS) {
                newMaterial->albedo = glm::vec3(color.r, color.g, color.b);
            }
            if (aimaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS) {
                newMaterial->albedo = glm::vec3(color.r, color.g, color.b);
            }

            if (aimaterial->Get(AI_MATKEY_METALLIC_FACTOR, newMaterial->metallic) != AI_SUCCESS) {
                aimaterial->Get(AI_MATKEY_SPECULAR_FACTOR, newMaterial->metallic);
            }
            if (aimaterial->Get(AI_MATKEY_ROUGHNESS_FACTOR, newMaterial->roughness) != AI_SUCCESS) {
                if (aimaterial->Get(AI_MATKEY_GLOSSINESS_FACTOR, newMaterial->roughness) == AI_SUCCESS) {
                    newMaterial->roughness = 1 - newMaterial->roughness;
                }
            }

            newMaterial->albedo_texture = loadMaterialTexture(aimaterial, aiTextureType_DIFFUSE, "texture_diffuse");
            newMaterial->metallic_texture = loadMaterialTexture(aimaterial, aiTextureType_METALNESS, "texture_metallic");
            newMaterial->roughness_texture = loadMaterialTexture(aimaterial, aiTextureType_DIFFUSE_ROUGHNESS, "texture_roughness");
            newMaterial->ao_texture = loadMaterialTexture(aimaterial, aiTextureType_AMBIENT_OCCLUSION, "texture_ao");

            newMaterial->normal_map = loadMaterialTexture(aimaterial, aiTextureType_NORMALS, "texture_normal");
            newMaterial->bump_map = loadMaterialTexture(aimaterial, aiTextureType_HEIGHT, "texture_bump");

            newMaterial->use_albedo_map = newMaterial->albedo_texture != nullptr;
            newMaterial->use_metallic_map = newMaterial->metallic_texture != nullptr;
            newMaterial->use_roughness_map = newMaterial->roughness_texture != nullptr;
            newMaterial->use_ao_map = newMaterial->ao_texture != nullptr;
            newMaterial->use_normal_map = newMaterial->normal_map != nullptr;
            newMaterial->use_bump_map = newMaterial->bump_map != nullptr;

            materials.push_back(newMaterial);
        }
    }

    // from https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/model.h
    unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma)
    {
        std::string filename = std::string(path);
        filename = directory + '/' + filename;

        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            GLenum format;
            if (nrComponents == 1)
                format = GL_RED;
            else if (nrComponents == 3)
                format = GL_RGB;
            else if (nrComponents == 4)
                format = GL_RGBA;

            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            LEI_WARN("Texture failed to load at path: " + std::string(path));
            stbi_image_free(data);
        }

        return textureID;
    }
}