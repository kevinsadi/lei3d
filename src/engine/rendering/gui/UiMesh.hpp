#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace lei3d
{
    class TGATexture;
	class Shader;

	class UiMesh
	{
	public:
		struct Vec2
		{
			float x, y;
		};

		struct Vertex
		{
			Vec2 m_position;
			Vec2 m_texCoords;

			Vertex(Vec2 position, Vec2 texCoords = {-1, -1})
				: m_position(position), m_texCoords(texCoords)
			{
			}
		};

		std::vector<Vertex> m_vertices;
		std::vector<unsigned int> m_indices;
		int m_textureID;
		
		UiMesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, int textureID = -1);
		~UiMesh();
		
		void Draw(Shader* shader) const;

	private:
		unsigned int VAO, VBO, EBO;

		void SetupBuffers();
	};

} // namespace lei3d