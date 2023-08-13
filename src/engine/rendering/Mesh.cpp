#include "Mesh.hpp"

#include "logging/GLDebug.hpp"

namespace lei3d
{
    Mesh::Mesh()
    {
        // ::clown emoticon::
    }

    Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        setupMesh();
    }

    Mesh::~Mesh()
    {
        GLCall(glDeleteBuffers(1, &VAO));
        GLCall(glDeleteBuffers(1, &EBO));
        GLCall(glDeleteBuffers(1, &VBO));
    }

    void Mesh::setupMesh()
    {
        GLCall(glGenVertexArrays(1, &VAO));
        GLCall(glGenBuffers(1, &VBO));
        GLCall(glGenBuffers(1, &EBO));

        GLCall(glBindVertexArray(VAO));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
        GLCall(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW));

        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                    &indices[0], GL_STATIC_DRAW));

        // vertex positions
        GLCall(glEnableVertexAttribArray(0));
        GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0));
        // vertex normals
        GLCall(glEnableVertexAttribArray(1));
        GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal)));
        // vertex texture coords
        GLCall(glEnableVertexAttribArray(2));
        GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords)));

        //DON'T UNBIND THE BUFFERS HERE (it causes it to not render fsr I don't understand)
        //GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
        //GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GLCall(glBindVertexArray(0));
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
    void Mesh::Draw(Shader &shader) const
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
        shader.bind();
        glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        shader.unbind();
        glBindVertexArray(0);

        // set back to default
        glActiveTexture(GL_TEXTURE0);
    }

}