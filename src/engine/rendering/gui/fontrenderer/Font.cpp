#include "Font.hpp"

#include <fstream>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "TGATexture.hpp"
#include "logging/Log.hpp"

namespace lei3d
{

	void Font::Load(const std::string& fontName)
	{
		std::string texFilename = "./data/fonts/" + fontName + "/" + fontName + ".tga";
		m_texture = new TGATexture(texFilename);

		std::string filename = "./data/fonts/" + fontName + "/" + fontName + ".fnt";
		std::ifstream file(filename);

		if (!file.is_open())
		{
			LEI_ERROR("Unable to open file.");
		}

		std::string line;
		while (std::getline(file, line))
		{
			std::vector<int>numbers;
			std::regex regex("-?\\d+");
			std::sregex_token_iterator iter(line.begin(), line.end(), regex);
			std::sregex_token_iterator end;

			while (iter != end)
			{
				numbers.push_back(std::stoi(*iter));
				++iter;
			}

			if (numbers.size() != 9)
			{
				if (line.substr(0, 4) == "info")
				{
					m_fontSize = numbers[0];
				}
				else if (line.substr(0, 6) == "common")
				{
					m_lineHeightPx = numbers[0];
				}
				continue;
			}

			Character thisChar = Character();
			// [0, 1]
			thisChar.TopLeft = glm::vec2(numbers[1] / (float)m_texture->m_width, numbers[2] / (float)m_texture->m_height);
			thisChar.BottomRight = glm::vec2((numbers[1] + numbers[3]) / (float)m_texture->m_width, (numbers[2] + numbers[4]) / (float)m_texture->m_height);

			// relative to line height
			thisChar.Size = glm::vec2(numbers[3] / m_lineHeightPx, numbers[4] / m_lineHeightPx);
			thisChar.XOffset = numbers[5] / m_lineHeightPx;
			thisChar.YOffset = numbers[6] / m_lineHeightPx;
			thisChar.XAdvance = numbers[7] / m_lineHeightPx;

			m_characters[numbers[0]] = thisChar;
		}
	}

	Font::Font(const std::string& fontname)
		: m_texture(nullptr)
		, m_fontName(fontname)
	{
		Load(fontname);
	}

	Font::~Font()
	{
		delete m_texture;
	}
}
