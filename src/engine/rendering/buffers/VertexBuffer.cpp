#include "VertexBuffer.hpp"

#include "logging/GLDebug.hpp"
#include "logging/Log.hpp"

VertexBuffer::VertexBuffer(const void* data, unsigned int size, GLenum usage)
{
	GLCall(glGenBuffers(1, &m_BufferID));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_BufferID));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, usage));
}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_BufferID));
}

void VertexBuffer::updateBuffer(GLintptr offset, GLsizeiptr size, GLvoid* data)
{
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, offset, size, data));
}

void VertexBuffer::bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_BufferID));
}

void VertexBuffer::unbind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
