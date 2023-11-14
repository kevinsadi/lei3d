#include "TGATexture.hpp"

#include <fstream>
#include <iostream>
#include <vector>

#include "glad/glad.h"
#include "logging/GLDebug.hpp"
#include "logging/Log.hpp"

namespace lei3d
{
	TGATexture::TGATexture(const std::string& fileName)
	{
		LoadTga(fileName);
		//LoadTexture();
	}

	void TGATexture::LoadTga(const std::string& filename)
	{
		GLenum	format;

		// Open the TGA file
		std::ifstream file(filename, std::ios::binary);
		if (!file)
		{
			std::cerr << "Failed to open TGA file: " << filename << std::endl;
			return;
		}

		// Read the TGA header
		char header[18];
		file.read(header, 18);

		// Extract width, height, and pixel format from the header
		m_width = header[12] + (header[13] << 8);
		m_height = header[14] + (header[15] << 8);
		int bitsPerPixel = header[16];

		// Determine the pixel format based on bits per pixel
		if (bitsPerPixel == 24)
		{
			format = GL_RGB;
		}
		else if (bitsPerPixel == 32)
		{
			format = GL_RGBA;
		}
		else
		{
			std::cerr << "Unsupported TGA format: " << bitsPerPixel << " bits per pixel" << std::endl;
			return;
		}

		// Read pixel data
		m_data = std::vector<unsigned char>(m_width * m_height * (bitsPerPixel / 8));
		file.read(reinterpret_cast<char*>(m_data.data()), m_data.size());
		file.close();

		// Create an OpenGL texture
		glGenTextures(1, &m_GlID);
		glBindTexture(GL_TEXTURE_2D, m_GlID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, m_data.data());

		// Set texture parameters (you can modify these as needed)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}


	/*
	char* TGATexture::ReadFile(std::string filePath, int* fileSize)
	{
		std::ifstream file(filePath, std::ios::binary);

		if (!file)
		{
			LEI_ERROR("Failed to open the file.");
			return new char[1];
		}

		file.seekg(0, std::ios::end);
		*fileSize = file.tellg();
		file.seekg(0, std::ios::beg);

		char* buffer = new char[*fileSize + 1];

		if (!file.read(buffer, *fileSize))
		{
			LEI_ERROR("Failed to read the file.");
			delete[] buffer;
			return new char[1];
		}

		file.close();

		return buffer;
	}

	void TGATexture::LoadTga(const std::string& filePath)
	{
		std::ifstream file(filePath, std::ios::binary);
		if (!file.is_open())
		{
			std::cerr << "Failed to open file: " << filePath << std::endl;
			return;
		}

		// Read TGA header
		uint8_t	 idLength;
		uint8_t	 colorMapType;
		uint8_t	 imageType;
		uint16_t xOrigin, yOrigin;
		uint8_t	 imageDescription;

		file.read(reinterpret_cast<char*>(&idLength), sizeof(idLength));
		file.read(reinterpret_cast<char*>(&colorMapType), sizeof(colorMapType));
		assert(colorMapType == 0);
		file.read(reinterpret_cast<char*>(&imageType), sizeof(imageType));
		assert(imageType == 2);
		file.ignore(5); // Color map specification, doesn't matter

		file.read(reinterpret_cast<char*>(&xOrigin), sizeof(xOrigin));
		file.read(reinterpret_cast<char*>(&yOrigin), sizeof(yOrigin));

		file.read(reinterpret_cast<char*>(&m_width), sizeof(m_width));
		file.read(reinterpret_cast<char*>(&m_height), sizeof(m_height));

		file.read(reinterpret_cast<char*>(&m_bytesPerPixel), sizeof(m_bytesPerPixel));
		m_bytesPerPixel /= 8;

		file.read(reinterpret_cast<char*>(&imageDescription), sizeof(imageDescription));

		// Read image data
		m_data = std::vector<uint8_t>(m_width * m_height * m_bytesPerPixel);
		file.read(reinterpret_cast<char*>(m_data.data()), m_data.size());
	}

	void TGATexture::LoadTexture()
	{
		glActiveTexture(GL_TEXTURE0);
		
		glGenTextures(1, &m_GlID);
		glBindTexture(GL_TEXTURE_2D, m_GlID);

		GLenum format = (m_bytesPerPixel == 4) ? GL_RGBA : GL_RGB;

		glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, GL_BGRA, GL_UNSIGNED_BYTE, m_data.data());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, m_GlID);
	}*/
}
