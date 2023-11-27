#define MINIAUDIO_IMPLEMENTATION
#include "audio/AudioPlayer.hpp"

namespace lei3d
{
	// static
	// std::unordered_map<std::string, ma_sound*> _sounds_on_loop = std::unordered_map<std::string, ma_sound*>();

	AudioPlayer& AudioPlayer::GetInstance()
	{
		static AudioPlayer instance;
		return instance;
	}

	AudioPlayer::AudioPlayer()
	{
		ma_result result;
		m_AudioEngine = std::make_unique<ma_engine>();
		_sounds_on_loop = std::make_unique<std::unordered_map<std::string, ma_sound*>>();
		ma_engine_config engineConfig;

		engineConfig = ma_engine_config_init();

		result = ma_engine_init(&engineConfig, m_AudioEngine.get());
		if (result != MA_SUCCESS)
		{
			LEI_ERROR("AudioPlayer: Unable to initialize audio engine");
		}
		
	}

	AudioPlayer::~AudioPlayer()
	{
		ma_engine_uninit(m_AudioEngine.get());
	}

	void AudioPlayer::PlayMusic(const std::string& musicName, float volume)
	{
		if (_sounds_on_loop.get()->contains(musicName))
		{
			LEI_ERROR("AudioPlayer: Music sound already playing!");
			return;
		}
		std::string musicPath = "data/audio/" + musicName;

		std::string sfxPath = "data/audio/sfx/" + musicName;
		// ma_engine_play_sound(m_AudioEngine.get(), musicPath.c_str(), NULL);

		ma_result result;
		ma_sound* sound = (ma_sound*)malloc(sizeof(ma_sound));

		result = ma_sound_init_from_file(m_AudioEngine.get(), musicPath.c_str(), 0, NULL, NULL, sound);
		if (result != MA_SUCCESS)
		{
			LEI_ERROR("AudioPlayer: Unable to create music sound");
			free(sound);
			return;
		}
		ma_sound_set_volume(sound, volume);
		ma_sound_set_looping(sound, MA_TRUE);
		ma_sound_start(sound);
		_sounds_on_loop.get()->insert({ musicName, sound });
	}

	void AudioPlayer::StopMusic(const std::string& musicName)
	{
		if (!_sounds_on_loop.get()->contains(musicName))
		{
			LEI_ERROR("AudioPlayer: Music sound " + musicName + " not on loop");
			return;
		}
		ma_sound *sound = _sounds_on_loop.get()->at(musicName);
		ma_sound_uninit(sound);
		free(sound);
		_sounds_on_loop.get()->erase(musicName);
	}

	void AudioPlayer::PlaySFX(const std::string& sfxName)
	{
		std::string sfxPath = "data/audio/sfx/" + sfxName;
		ma_engine_play_sound(m_AudioEngine.get(), sfxPath.c_str(), NULL);
	}

	// std::unordered_set<std::string> AudioPlayer::_sounds_on_loop = std::unordered_set<std::string>();

	// void AudioPlayer::PlaySFXForMilliseconds(
	// 	const std::string& sfxName,
	// 	ma_uint64 milliseconds,
	// 	float volume,
	// 	ma_uint64 fadeInLengthInMilliseconds,
	// 	ma_uint64 fadeOutLengthInMilliseconds)
	// {
	// 	ma_result result;
	// 	std::string sfxPath = "data/audio/sfx/" + sfxName + ".mp3";
	// 	ma_sound* sound = (ma_sound*)malloc(sizeof(ma_sound));
	// 	result = ma_sound_init_from_file(m_AudioEngine.get(), sfxPath.c_str(), 0, NULL, NULL, sound);
	// 	if (result != MA_SUCCESS)
	// 	{
	// 		LEI_ERROR("Could not load " + sfxPath + " from file!");
	// 		free(sound);
	// 		return; // Failed to load sound.
	// 	}

	// 	if (_sounds_on_loop.contains(sfxPath))
	// 	{
	// 		ma_sound_uninit(sound);
	// 		free(sound);
	// 		return;
	// 	}
	// 	_sounds_on_loop.insert(sfxPath);

	// 	ma_sound_set_volume(sound, volume);
	// 	ma_sound_set_fade_in_milliseconds(sound, 0.0f, volume, fadeInLengthInMilliseconds);
	// 	ma_sound_start(sound);
	// 	ma_sound_set_looping(sound, MA_TRUE);

	// 	std::thread timer_thread(AudioPlayer::timer, sfxPath, milliseconds, sound, volume, fadeOutLengthInMilliseconds);

	// 	timer_thread.detach();
	// }

	// void AudioPlayer::timer(std::string sfxPath, ma_uint64 milliseconds, ma_sound* sound, float volume, ma_uint64 fadeOutLengthInMilliseconds)
	// {
	// 	/*
	// 	Trying an approach outlined here: https://linuxhint.com/timer-function-cpp/
	// 	Hopefully it's cross-platform
	// 	*/
	// 	clock_t now = clock();
	// 	if (fadeOutLengthInMilliseconds > milliseconds)
	// 	{
	// 		fadeOutLengthInMilliseconds = milliseconds;
	// 	}
	// 	ma_uint64 ms = milliseconds * 1000;
	// 	while (clock() - now < ms - fadeOutLengthInMilliseconds * 1000)
	// 		;
	// 	clock_t now2 = clock();
	// 	ma_sound_set_fade_in_milliseconds(sound, volume, 0.0f, fadeOutLengthInMilliseconds);
	// 	fadeOutLengthInMilliseconds *= 1000;
	// 	while (clock() - now2 < fadeOutLengthInMilliseconds)
	// 		;
	// 	clock_t thing = clock();
	// 	ma_sound_uninit(sound);
	// 	free(sound);
	// 	_sounds_on_loop.erase(sfxPath);
	// }

	// void AudioPlayer::PlaySFXForMilliseconds(const std::string& sfxName, ma_uint64 milliseconds, float volume)
	// {
	// 	PlaySFXForMilliseconds(sfxName, milliseconds, volume, 0, 0);
	// }

	// void AudioPlayer::PlaySFXForMilliseconds(const std::string& sfxName, ma_uint64 milliseconds)
	// {
	// 	PlaySFXForMilliseconds(sfxName, milliseconds, 1.0f, 0, 0);
	// }

	// void AudioPlayer::PlaySFXForSeconds(
	// 	const std::string& sfxName,
	// 	ma_uint64 seconds,
	// 	float volume,
	// 	ma_uint64 fadeInLengthInSeconds,
	// 	ma_uint64 fadeOutLengthInSeconds)
	// {
	// 	AudioPlayer::PlaySFXForMilliseconds(sfxName, seconds * 1000, volume, fadeInLengthInSeconds * 1000, fadeOutLengthInSeconds * 1000);
	// }

	// void AudioPlayer::PlaySFXForSeconds(
	// 	const std::string& sfxName,
	// 	ma_uint64 seconds,
	// 	float volume)
	// {
	// 	AudioPlayer::PlaySFXForMilliseconds(sfxName, seconds * 1000, volume, 0, 0);
	// }

	// void AudioPlayer::PlaySFXForSeconds(
	// 	const std::string& sfxName,
	// 	ma_uint64 seconds)
	// {
	// 	AudioPlayer::PlaySFXForMilliseconds(sfxName, seconds * 1000, 1.0f, 0, 0);
	// }
} // namespace lei3d