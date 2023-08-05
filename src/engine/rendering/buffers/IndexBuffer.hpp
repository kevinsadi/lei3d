#pragma once

#include <glad/glad.h>

class IndexBuffer {
private:
	unsigned int m_BufferID;	//Location of the buffer in memory
	unsigned int m_Count;	//The number of indices that this buffer holds
public:
	IndexBuffer(const unsigned int* data, unsigned int count, GLenum usage);
	~IndexBuffer();

	void bind() const;
	void unbind() const;

	inline unsigned int getCount() const { return m_Count; };
};