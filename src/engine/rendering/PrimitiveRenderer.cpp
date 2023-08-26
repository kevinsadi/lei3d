#include "PrimitiveRenderer.hpp"

#include "logging/GLDebug.hpp"

namespace lei3d
{
	PrimitiveRenderer::PrimitiveRenderer()
	{
	}

	void PrimitiveRenderer::initialize(float width, float height)
	{
		m_Width = width;
		m_Height = height;

		m_PrimitiveShader = Shader("./data/shaders/primitive.vert", "./data/shaders/primitive.frag");
	}
	void PrimitiveRenderer::drawLine(FlyCamera& camera, const glm::vec3& from, const glm::vec3& to, const glm::vec3& color, float thickness)
	{
		//Construct line of some thickness that always faces the camera.
		glm::vec3 lineDir = to - from;
		lineDir = glm::normalize(lineDir);

		glm::vec3 vOffset = glm::cross(lineDir, camera.Front());
		vOffset = 0.5f * thickness * glm::normalize(vOffset);

		glm::vec3 vertices[] = {
			from + vOffset,
			from - vOffset,
			to - vOffset,
			to + vOffset
		};

		unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

		VertexArray		   vao;
		VertexBuffer	   vbo(vertices, 4 * 3 * sizeof(GL_FLOAT), GL_STATIC_DRAW);
		VertexBufferLayout vbLayout;
		vbLayout.push<float>(3);
		vao.addBuffer(vbo, vbLayout);

		IndexBuffer ibo(indices, 6, GL_STATIC_DRAW);

		m_PrimitiveShader.setUniformMat4("u_Proj", camera.GetProj());
		m_PrimitiveShader.setUniformMat4("u_View", camera.GetView());
		m_PrimitiveShader.setUniformMat4("u_Model", glm::identity<glm::mat4>());

		m_PrimitiveShader.setVec3("u_Color", color);
		draw(vao, ibo);
	}

	void PrimitiveRenderer::draw(const VertexArray& va, const IndexBuffer& ib) const
	{
		va.bind();
		ib.bind();
		m_PrimitiveShader.bind();

		GLCall(glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr));

		m_PrimitiveShader.unbind();
		ib.unbind();
		va.unbind();
	}

	Shader& PrimitiveRenderer::GetShader()
	{
		return m_PrimitiveShader;
	}
} // namespace lei3d