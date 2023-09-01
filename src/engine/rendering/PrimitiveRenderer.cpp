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
		while (!m_DrawCalls.empty())
		{
			m_DrawCalls.pop();
		}

		m_PrimitiveShader = Shader("./data/shaders/primitive.vert", "./data/shaders/primitive.frag");
	}
	void PrimitiveRenderer::pushLine(FlyCamera& camera, const glm::vec3& from, const glm::vec3& to, const glm::vec3& color, float thickness)
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

		DrawData		   data;
		data.m_VAO = std::make_unique<VertexArray>();
		data.m_VBO = std::make_unique<VertexBuffer>(vertices, 4 * 3 * sizeof(GL_FLOAT), GL_STATIC_DRAW);
		data.m_IBO = std::make_unique<IndexBuffer>(indices, 6, GL_STATIC_DRAW);
		data.u_Color = color;
		;
		VertexBufferLayout vbLayout;
		vbLayout.push<float>(3);
		data.m_VAO->addBuffer(*data.m_VBO, vbLayout);

		m_DrawCalls.push(std::move(data));
	}

	void PrimitiveRenderer::drawAll(FlyCamera& camera)
	{
		m_PrimitiveShader.setUniformMat4("u_Proj", camera.GetProj());
		m_PrimitiveShader.setUniformMat4("u_View", camera.GetView());
		m_PrimitiveShader.setUniformMat4("u_Model", glm::identity<glm::mat4>());
		while (!m_DrawCalls.empty())
		{
			DrawData& data = m_DrawCalls.front();
			m_PrimitiveShader.setVec3("u_Color", data.u_Color);
			draw(*data.m_VAO, *data.m_IBO);
			m_DrawCalls.pop();
		}
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