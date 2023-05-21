#include "include/engine/Mesh.hpp"

namespace kek3d
{
    Mesh::Mesh()
    {
        // ::clown emoticon::
    }

    Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        setupMesh();
    }

    Mesh::~Mesh()
    {
        glDeleteBuffers(1, &VAO);
        glDeleteBuffers(1, &EBO);
        glDeleteBuffers(1, &VBO);
    }

    void Mesh::setupMesh()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
    
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  
        //std::cout << "got here lol" << std::endl;

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), 
                    &indices[0], GL_STATIC_DRAW);
        //std::cout << "got here lol" << std::endl;

        // vertex positions
        glEnableVertexAttribArray(0);	
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);	
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);	
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

        glBindVertexArray(0);
    }

    /**
    * I AM MAKING A WEIRD DESIGN DECISION HERE 
    * 
    * IN THE SHADERS WE PASS IN, WE ASSUME THE TEXTURES ARE FORMATTED LIKE
    * 
    * uniform sampler texture_1;
    * uniform sampler texture_2;
    * uniform sampler texture_3;
    * 
    * AND SO ON. HOPEFULLY I WILL HAVE DEFAULT SHADER TYPES TO MAKE OUR LIFE EASIER.
    */
    void Mesh::Draw(Shader &shader)
    {
        unsigned int textureNum = 1;

        for (unsigned int i = 0; i < textures.size(); i++) // very purposeful decision for i++ here, increment after first iteration, not before
        {
            glActiveTexture(GL_TEXTURE0 + i);

            // set shader's 2d uniform samplers to the textures in the mesh
            shader.setInt(("texture_" + std::to_string(textureNum)).c_str(), i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

        // actually draw the mesh now
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // set back to default
        glActiveTexture(GL_TEXTURE0);
    }

}