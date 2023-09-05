#pragma once

#include "components/Camera.hpp"

#include "rendering/Shader.hpp"
#include "rendering/buffers/Buffer.hpp"

#include <glm/glm.hpp>

#include <queue>
#include <vector>

namespace lei3d
{
	/*
	* This is a library for rendering primitives
	* It is mainly used for debug purposes
	*
	* How it works is you use the "push __" functions, which
	* will construct buffers on the GPU for a certain draw call
	*
	* When you call "drawAll()", the queue is drained and everything in it is rendered to the screen.
	* This is in order to sync primitive rendering with the rest of the game rendering, specifically, we want
	* to do primitive rendering after everything else so that it is not blocked by other objects.
	*/

	class PrimitiveRenderer
	{
		struct DrawData
		{
			//These can't be on the stack due to glDeleteBuffers getting called in destructor.
			std::unique_ptr<VertexArray>  m_VAO;
			std::unique_ptr<VertexBuffer> m_VBO; //It is really important that VertexBuffer stays in scope even though it is not an argument to Draw.
			std::unique_ptr<IndexBuffer>  m_IBO;

			//Add Uniform Types Here
			glm::vec3 u_Color;
		};

	private:
		float  m_Width, m_Height;
		Shader m_PrimitiveShader;

		std::queue<DrawData> m_DrawCalls;

	public:
		PrimitiveRenderer();
		void initialize(float width, float height);

		void drawAll(Camera& camera);

		void pushLine(Camera& camera, const glm::vec3& from, const glm::vec3& to, const glm::vec3& color, float thickness = 0.1f);

		Shader& GetShader();

	private:
		void draw(const VertexArray& va, const IndexBuffer& ib) const;
	};
} // namespace lei3d
