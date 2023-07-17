#ifndef MESH
#define MESH

#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "engine/Shader.hpp"
#include "engine/Material.hpp"

namespace lei3d
{
    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
    };

    class Mesh
    {
    public:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::shared_ptr<Material> material;
        
        Mesh();
        Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::shared_ptr<Material> material); // I'm a pass by reference man now
        ~Mesh();

        void Draw(Shader &shader); // taking value by reference for funsies, might switch it up back to pointers
    private:
        unsigned int VAO, VBO, EBO;

        void setupMesh();
    };

}

#endif // MESH