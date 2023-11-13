#include "AppSettings.hpp"

#include <fstream>
#include <sstream>
#include <algorithm>

namespace lei3d
{
	// default settings (if config file not found/on first start up)
	uint32_t AppSettings::Width = 1920;
	uint32_t AppSettings::Height = 1080;
	bool AppSettings::Fullscreen = false;
	uint32_t AppSettings::FOV = 55;
	float AppSettings::MouseSpeed = 0.1f;
	uint8_t AppSettings::ShadowQuality = 0;
	bool AppSettings::SSR = false;

	AppSettings& AppSettings::GetInstance()
	{
		static AppSettings instance;
		return instance;
	}

	void AppSettings::Initialize()
	{
		if (!ReadConfigFile())
		{
			WriteConfigFile();
		}
	}

	bool AppSettings::ReadConfigFile()
	{
		std::ifstream inconfig(filename);
		if (!inconfig)
		{
			// couldn't open file for some reason
			return false;
		}

		std::string line;
		std::vector<std::string> strings;
		while (std::getline(inconfig, line))
		{
			trim(line);

			if (line.empty())
			{
				continue;
			}

			// ignore if it's a comment
			if (line[0] == '#')
			{
				continue;
			}

			// split into name:value pairs
			std::istringstream stream(line);
			std::string val;
			while (std::getline(stream, val, ':'))
			{
				trim(val);
				strings.push_back(val);
			}

			if (strings.size() != 2)
			{
				fprintf(stderr, "Error reading config: value mismatch\n");
			}
			ParseConfigs(strings);

			strings.clear();
		}
		inconfig.close();

		return true;
	}

	void AppSettings::ParseConfigs(const std::vector<std::string>& pairs)
	{
		// there should only be two values in the vector
		// 1st string - name of setting to change
		// 2nd string - new value of setting

		if (pairs[0].compare("width") == 0)
		{
			Width = std::stoi(pairs[1]);
		}
		else if (pairs[0].compare("height") == 0)
		{
			Height = std::stoi(pairs[1]);
		}
		else if (pairs[0].compare("fullscreen") == 0)
		{
			Fullscreen = std::stoi(pairs[1]) == 1;
		}
		else if (pairs[0].compare("fov") == 0)
		{
			FOV = std::clamp(std::stoi(pairs[1]), 36, 89);
		}
		else if (pairs[0].compare("mousespeed") == 0)
		{
			MouseSpeed = std::clamp(std::stof(pairs[1]), 0.01f, 1.f);
		}
		else if (pairs[0].compare("shadowqual") == 0)
		{
			ShadowQuality = std::stoi(pairs[1]) == 0 ? 0 : 1;
		}
		else if (pairs[0].compare("ssr") == 0)
		{
			SSR = std::stoi(pairs[1]) == 1;
		}
	}

	void AppSettings::WriteConfigFile()
	{
		std::ofstream outconfig;
		outconfig.open(filename);

		outconfig << "# Lei3D config file\n\n";

		outconfig << "# Screen size\n";
		outconfig << "width:" << Width << "\n";
		outconfig << "height:" << Height << "\n";
		outconfig << "fullscreen:" << Fullscreen << "\n\n";

		outconfig << "fov:" << FOV << "\n";
		outconfig << "mousespeed:" << MouseSpeed << "\n\n";

		outconfig << "# Graphics settings\n";
		outconfig << "# shadow quality: 0 - medium, 1 - high\n";
		outconfig << "shadowqual:" << unsigned(ShadowQuality) << "\n";
		outconfig << "# enable/disable screen space reflections\n";
		outconfig << "ssr:" << SSR << "\n";

		outconfig.close();
	}

	void AppSettings::ltrim(std::string& s)
	{
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
			return !std::isspace(ch);
		}));
	}

	void AppSettings::rtrim(std::string& s)
	{
		s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
			return !std::isspace(ch);
		}).base(),
			s.end());
	}

	void AppSettings::trim(std::string& s)
	{
		rtrim(s);
		ltrim(s);
	}

} // namespace lei3d