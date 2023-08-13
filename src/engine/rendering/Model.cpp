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
        const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::string errorString = importer.GetErrorString();
            LEI_WARN("ERROR::ASSIMP::" + errorString);
            return;
        }
        m_Directory = path.substr(0, path.find_last_of('/'));

        processNode(scene->mRootNode, scene);
    }

    // REFER BACK TO THIS WHEN WE ARE MAKING ECS
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
        std::vector<Texture> textures;

        // process vertices from assimp to our own mesh component
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;

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
        if(mesh->mMaterialIndex >= 0)
        {
            // here is where our assumption of sampler names matters when we create our mesh
            // create materials
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex]; // refer to assimp data structures

            // this is a bit wonky, we're gonna have to experiment with this as we import more detailed meshes. ***BLENDER_USERS***
            // 1. diffuse maps
            std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
            // 2. specular maps
            std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
            // 3. normal maps
            std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
            textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
            // 4. height maps
            std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
            textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
        }  
        
        // return a mesh object created from the extracted mesh data
        return Mesh(vertices, indices, textures);
    }

    // get the materials that we want from the assimp mat and convert it to textures array that is returned
    std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName)
    {
        std::vector<Texture> textures;
        for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);

            bool skip = false;
            for (unsigned int j = 0; j < m_TexturesLoaded.size(); j++) // for every texture we've loaded so far
            {
                if (std::strcmp(str.C_Str(), m_TexturesLoaded[j].path.c_str()) == 0)
                {
                    skip = true;
                }
            }

            if (!skip)
            {
                Texture texture;
                texture.id = TextureFromFile(str.C_Str(), m_Directory);
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                m_TexturesLoaded.push_back(texture);
            }
        }
        return textures;
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

        for (Mesh mesh : m_Meshes)
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