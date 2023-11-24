#pragma once

#include <string>
#include <vector>

namespace lei3d
{

	class AppSettings
	{
	public:

		static AppSettings& GetInstance();
		~AppSettings() = default;

		void Initialize();

		static uint32_t Width;
		static uint32_t Height;
		static bool Fullscreen;

		static uint32_t FOV;
		static float MouseSpeed;

		// shadow quality: 0 - medium (2048), 1 - high (4096)
		static uint8_t ShadowQuality;
		static bool SSR;

	private:
		AppSettings() {}

		bool ReadConfigFile();
		void ParseConfigs(const std::vector<std::string>& pairs);
		void WriteConfigFile();

		// string parse helpers
		void ltrim(std::string& s);
		void rtrim(std::string& s);
		void trim(std::string& s);

		const std::string filename = "lei-config.ini";
	};

} // namespace lei3d
