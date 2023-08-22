#include "VertexArray.hpp"

#include "logging/GLDebug.hpp"
#include "logging/Log.hpp"

#include <glad/glad.h>

VertexArray::VertexArray() {
	GLCall(glGenVertexArrays(1, &m_ArrayID));
}

VertexArray::~VertexArray() {
	GLCall(glDeleteVertexArrays(1, &m_ArrayID));
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
	// Bind the vertex array and the buffer
	bind();
	vb.bind();

	std::vector<VertexBufferAttribute> elements = layout.getElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++) {
		const VertexBufferAttribute& element = elements[i];

		/*For an attribute:
		index - The index of the attribute
		size - The number of components in the attribute (the count: for example the position attribute has size 2 or 3)
		type - The data type of the components in this attribute
		normalized - Whether the data should be normalized
		stride - The number of bytes in between attributes (usually the size of the entire vertex)
		pointer - The byte offset of the attribute inside the vertex
		*/

		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*)offset));

		// Enables the attribute at the given index
		GLCall(glEnableVertexAttribArray(i));

		offset += element.count * VertexBufferAttribute::getSizeOfType(element.type);
	}
}

void VertexArray::bind() const {
	GLCall(glBindVertexArray(m_ArrayID));
}

void VertexArray::unbind() const {
	GLCall(glBindVertexArray(0));
}