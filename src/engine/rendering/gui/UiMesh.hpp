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

		struct Vec4
		{
			float x, y, z, w;
		};

		struct Vertex
		{
			Vec2 m_position;
			Vec4 m_color;
			Vec2 m_texCoords;

			Vertex(Vec2 position, Vec4 color, Vec2 texCoords)
				: m_position(position), m_color(color), m_texCoords(texCoords)
			{
			}
		};

		std::vector<Vertex> m_vertices;
		std::vector<unsigned int> m_indices;
		const TGATexture& m_texture;
		
		UiMesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, const TGATexture& texture);
		~UiMesh();

		void Draw() const;
		void Draw(Shader& shader) const;

	private:
		unsigned int VAO, VBO, EBO;

		void SetupBuffers();
	};

} // namespace lei3d