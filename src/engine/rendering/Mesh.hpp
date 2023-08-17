#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "rendering/Shader.hpp"
#include "rendering/Material.hpp"

namespace lei3d
{
    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
        glm::vec3 Tangent;
    };

    class Mesh
    {
    public:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::shared_ptr<Material> material;
        
        Mesh();
        Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::shared_ptr<Material>& material);
        ~Mesh();

        void Draw(Shader &shader) const; // taking value by reference for funsies, might switch it up back to pointers
    private:
        unsigned int VAO, VBO, EBO;

        void setupMesh();
    };

}