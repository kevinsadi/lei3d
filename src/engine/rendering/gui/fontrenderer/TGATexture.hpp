#pragma once
#include <string>
#include <vector>

namespace lei3d
{
	class TGATexture
	{
	public:
		int m_width;
		int m_height;
		int m_bytesPerPixel;
		std::vector<unsigned char> m_data;
		unsigned m_GlID;

		TGATexture(std::string fileName);

	private:
		char* ReadFile(std::string filePath, int* fileSize);

		void LoadTga(const std::string& filePath);
		//void LoadTexture();
	};
}
