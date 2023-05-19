#include <vector>
#include <string>

#include <glm/glm.hpp>

#include "include/engine/Shader.hpp"

namespace kek3d
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
    };

    class Mesh
    {
    public:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;
        
        Mesh();
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
        ~Mesh();

        void Draw(Shader &shader);
    private:
        unsigned int VAO, VBO, EBO;

        void setupMesh();
    };

}