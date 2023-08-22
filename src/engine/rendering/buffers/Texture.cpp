#include "Texture.hpp"

#include "logging/GLDebug.hpp"
#include "logging/Log.hpp"

#include <stb_image.h>

namespace lei3d {
Texture::Texture(const char* imgPath) {
	// Create a texture buffer on GPU to store the image.
	GLCall(glGenTextures(1, &m_ID));
	bind();

	// Set default parameter values for texture (these can be changed using setParameteri function)

	// Clamp overflowed (s, t)
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	// Linear Filtering
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	// Load the image
	unsigned char* img = stbi_load(imgPath, &m_Width, &m_Height, &m_nChannels, STBI_rgb);
	GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
	LEI_ASSERT(img != nullptr, "Could Not Load Image! ", imgPath);
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, img));
	stbi_image_free(img);  // Don't need the image buffer once it's loaded into the texture.

	LEI_INFO("Width: {}", m_Width);
	LEI_INFO("Height: {}", m_Height);
	unbind();
}

void Texture::setParameteri(GLenum name, GLint value) const {
	GLCall(glTexParameteri(GL_TEXTURE_2D, name, value))
}

void Texture::bind() const {
	GLCall(glActiveTexture(GL_TEXTURE0));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_ID));
}

void Texture::unbind() const {
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

int Texture::width() const {
	return m_Width;
}

int Texture::height() const {
	return m_Height;
}

unsigned int Texture::id() const {
	return m_ID;
}
}  // namespace lei3d
