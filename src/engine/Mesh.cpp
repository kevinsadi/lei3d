#include "engine/Mesh.hpp"

namespace lei3d
{
    Mesh::Mesh()
    {
        // ::clown emoticon::
    }

    Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::shared_ptr<Material> material)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->material = material;

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

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), 
                    &indices[0], GL_STATIC_DRAW);

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

        material->bind(shader, textureNum);

        // actually draw the mesh now
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        material->unbind(textureNum);
        // set back to default
        glActiveTexture(GL_TEXTURE0);
    }

}