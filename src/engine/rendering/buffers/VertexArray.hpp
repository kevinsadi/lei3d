#pragma once

#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"

// Specifies a vertex buffer and how it's layout (how the vertex attributes are arranged inside the buffer
class VertexArray
{
private:
	unsigned int m_ArrayID;

public:
	VertexArray();
	~VertexArray();

	void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
	void bind() const;
	void unbind() const;
};