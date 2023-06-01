#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "include/engine/Shader.hpp"

namespace lei3d
{
    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
    };

    struct Texture
    {
        unsigned int id;
        std::string type;
        std::string path;
    };

    class Mesh
    {
    public:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;
        
        Mesh();
        Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures); // I'm a pass by reference man now
        ~Mesh();

        void Draw(Shader &shader); // taking value by reference for funsies, might switch it up back to pointers
    private:
        unsigned int VAO, VBO, EBO;

        void setupMesh();
    };

}