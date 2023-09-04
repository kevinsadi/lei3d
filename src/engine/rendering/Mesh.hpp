#pragma once

#include "rendering/Material.hpp"
#include "rendering/Shader.hpp"

#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace lei3d
{

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
		glm::vec3 Tangent;
	};

	enum RenderFlag {
		None = 0x00000000,
		BindImages = 0x00000001,
		Opaque = 0x00000002,
		AlphaMask = 0x00000004
	};

	class Mesh
	{
	public:
		std::vector<Vertex>		  vertices;
		std::vector<unsigned int> indices;
		Material* material;

		Mesh();
		Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, Material* material);
		~Mesh();

		void Draw(Shader& shader, RenderFlag flags, uint32_t bindLocation) const;

	private:
		unsigned int VAO, VBO, EBO;

		void setupMesh();
	};

} // namespace lei3d