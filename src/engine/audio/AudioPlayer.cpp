#define MINIAUDIO_IMPLEMENTATION
#include "audio/AudioPlayer.hpp"

namespace lei3d
{
	AudioPlayer* AudioPlayer::s_AudioPlayer = nullptr;

	AudioPlayer::AudioPlayer()
	{
		ma_result result;
		m_AudioEngine = std::make_unique<ma_engine>();
		ma_engine_config engineConfig;

		engineConfig = ma_engine_config_init();

		result = ma_engine_init(&engineConfig, m_AudioEngine.get());
		if (result != MA_SUCCESS)
		{
			LEI_ERROR("AudioPlayer: Unable to initialize audio engine");
		}

		//ma_engine_play_sound(m_AudioEngine.get(), "data/audio/Ethereal_Surg_8-17.mp3", NULL);

		if (s_AudioPlayer)
		{
			LEI_ERROR("Multiple instances detected. Only one AudioPlayer should exist.");
		}

		s_AudioPlayer = this;
	}

	AudioPlayer::~AudioPlayer()
	{
		ma_engine_uninit(m_AudioEngine.get());
	}

	AudioPlayer& AudioPlayer::GetAudioPlayer()
	{
		return *(s_AudioPlayer);
	}

	void AudioPlayer::PlayMusic(const std::string& musicName)
	{
		std::string musicPath = "data/audio/" + musicName + ".mp3";

		ma_engine_play_sound(s_AudioPlayer->m_AudioEngine.get(), musicPath.c_str(), NULL);
	}

	void AudioPlayer::PlaySFX(const std::string& sfxName)
	{
		std::string sfxPath = "data/audio/sfx/" + sfxName + ".mp3";
		ma_engine_play_sound(s_AudioPlayer->m_AudioEngine.get(), sfxPath.c_str(), NULL);
	}

	/**
	 * This method is pulled directly from miniaudio's example code for their simple mixing.
	 * @param 
	 * @param
	 * @param
	 * @param frameCount number of frames to count
	 */
	void AudioPlayer::dataCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
	{
		ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
		if (pDecoder == NULL)
		{
			return;
		}

		/* Reading PCM frames will loop based on what we specified when called ma_data_source_set_looping(). */
		ma_data_source_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);

		(void)pInput;
	}

	void AudioPlayer::PlaySFXForMilliseconds(const std::string& sfxName, long milliseconds)
	{
		ma_result		 result;
		//ma_decoder		 decoder;
		//ma_device_config deviceConfig;
		//ma_device		 device;
		//ma_uint64		 framesRead = milliseconds;

		std::string sfxPath = "data/audio/sfx/" + sfxName + ".mp3";

		ma_sound sound;

		result = ma_sound_init_from_file(s_AudioPlayer->m_AudioEngine.get(), sfxPath.c_str(), 0, NULL, NULL, &sound);
		if (result != MA_SUCCESS)
		{
			std::cout << "ERROR: Could not load " << sfxPath << " from file!" << std::endl;
			return; // Failed to load sound.
		}

		std::cout << "RESULT: "<< result << "\t\tWe made it this far at least!" << std::endl;

		ma_sound_start(&sound);

		std::cout << "OKIE SOUND START GOT CALLED!" << std::endl;

	}
} // namespace lei3d