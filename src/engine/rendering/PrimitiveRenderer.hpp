#pragma once

#include "core/FlyCamera.hpp"

#include "rendering/Shader.hpp"
#include "rendering/buffers/Buffer.hpp"

#include <glm/glm.hpp>

namespace lei3d
{
	/*
	* This is a library for rendering primitives
	* It is mainly used for debug purposes
	*/
	class PrimitiveRenderer
	{
	private:
		float  m_Width, m_Height;
		Shader m_PrimitiveShader;

	public:
		PrimitiveRenderer();
		void initialize(float width, float height);
		void drawLine(FlyCamera& camera, const glm::vec3& from, const glm::vec3& to, const glm::vec3& color, float thickness = 0.1f);

		Shader& GetShader();

	private:
		void draw(const VertexArray& va, const IndexBuffer& ib) const;
	};
} // namespace lei3d
