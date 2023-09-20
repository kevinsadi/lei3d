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

	void AudioPlayer::PlaySFXForMilliseconds(
		const std::string& sfxName,
		long milliseconds,
		float volume,
		ma_uint64 fadeInLengthInMilliseconds,
		ma_uint64 fadeOutLengthInMilliseconds
	)
	{
		ma_result result;

		std::string sfxPath = "data/audio/sfx/" + sfxName + ".mp3";

		ma_sound sound;

		result = ma_sound_init_from_file(s_AudioPlayer->m_AudioEngine.get(), sfxPath.c_str(), 0, NULL, NULL, &sound);
		if (result != MA_SUCCESS)
		{
			std::cout << "ERROR: Could not load " << sfxPath << " from file!" << std::endl;
			return; // Failed to load sound.
		}

		ma_sound_set_volume(&sound, volume);
		ma_sound_set_fade_in_milliseconds(&sound, 0.0f, volume, fadeInLengthInMilliseconds);
		ma_sound_start(&sound);
		ma_sound_set_looping(&sound, MA_TRUE);
		std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds - fadeOutLengthInMilliseconds));
		ma_sound_set_fade_in_milliseconds(&sound, volume, 0.0f, fadeOutLengthInMilliseconds);
		std::this_thread::sleep_for(std::chrono::milliseconds(fadeOutLengthInMilliseconds));
		ma_sound_set_looping(&sound, MA_FALSE);
		ma_sound_uninit(&sound);

	}

	void AudioPlayer::PlaySFXForMilliseconds(const std::string& sfxName, long milliseconds, float volume)
	{
		PlaySFXForMilliseconds(sfxName, milliseconds, volume, 0, 0);
	}

	void AudioPlayer::PlaySFXForMilliseconds(const std::string& sfxName, long milliseconds)
	{
		PlaySFXForMilliseconds(sfxName, milliseconds, 1.0f, 0, 0);
	}

	void AudioPlayer::PlaySFXForSeconds(
		const std::string& sfxName,
		long seconds,
		float volume,
		ma_uint64 fadeInLengthInSeconds,
		ma_uint64 fadeOutLengthInSeconds
	)
	{
		AudioPlayer::PlaySFXForMilliseconds(sfxName, seconds * 1000, volume, fadeInLengthInSeconds * 1000, fadeOutLengthInSeconds * 1000);
	}

	void AudioPlayer::PlaySFXForSeconds(
		const std::string& sfxName,
		long			   seconds,
		float			   volume)
	{
		AudioPlayer::PlaySFXForMilliseconds(sfxName, seconds * 1000, volume, 0, 0);
	}

	void AudioPlayer::PlaySFXForSeconds(
		const std::string& sfxName,
		long			   seconds)
	{
		AudioPlayer::PlaySFXForMilliseconds(sfxName, seconds * 1000, 1.0f, 0, 0);
	}
} // namespace lei3d