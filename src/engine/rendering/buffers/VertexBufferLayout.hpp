#pragma once

#include "logging/Log.hpp"

#include <glad/glad.h>

#include <vector>

// One attribute
struct VertexBufferAttribute
{
	unsigned int  type;
	unsigned int  count;
	unsigned char normalized;

	static unsigned int getSizeOfType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT:
				return 4;
			case GL_UNSIGNED_INT:
				return 4;
			case GL_UNSIGNED_BYTE:
				return 1;
		}

		LEI_ASSERT(false); // Unrecognized Type
		return 0;
	}
};

// The entire layout for the vertex in the vertex buffer
class VertexBufferLayout
{
private:
	std::vector<VertexBufferAttribute> m_Attributes;
	unsigned int					   m_Stride;

public:
	VertexBufferLayout()
		: m_Stride(0) {}

	// Allocates space for an attribute in the layout with the number of components specified.
	void push(unsigned int count, unsigned int type)
	{
		m_Attributes.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * VertexBufferAttribute::getSizeOfType(type);
	}

	inline const std::vector<VertexBufferAttribute> getElements() const { return m_Attributes; };
	inline unsigned int								getStride() const { return m_Stride; };
};

