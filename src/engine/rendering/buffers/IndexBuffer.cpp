#include "IndexBuffer.hpp"

#include "logging/Log.hpp"
#include "logging/GLDebug.hpp"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count, GLenum usage)
	: m_Count(count)
{
	GLCall(glGenBuffers(1, &m_BufferID));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(data[0]), data, usage));
}

IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_BufferID));
}

void IndexBuffer::bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID));
}

void IndexBuffer::unbind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
