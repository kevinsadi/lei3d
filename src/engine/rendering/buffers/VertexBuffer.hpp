#pragma once

#include <glad/glad.h>

class VertexBuffer {
private:
	unsigned int m_BufferID;	//Location of the buffer in memory
public:
	VertexBuffer(const void* data, unsigned int size, GLenum usage);
	~VertexBuffer();

	void updateBuffer(GLintptr offset, GLsizeiptr size, GLvoid* data);

	void bind() const;
	void unbind() const;
};