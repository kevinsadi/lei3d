#pragma once

#include <iostream>
#include <thread>
#include <chrono>
#include <stdlib.h>
#include <unordered_set>

#include "miniaudio.h"
#include "logging/Log.hpp"
#include "core/Scene.hpp"
#include "components/TimerComponent.hpp"

namespace lei3d
{
	class AudioPlayer
	{
	public:
		AudioPlayer();
		~AudioPlayer();

		static AudioPlayer& GetInstance();

		void PlayMusic(const std::string& musicName, float volume);
		void PlaySFX(const std::string& sfxName);

		std::unique_ptr<ma_engine> m_AudioEngine;
		std::unique_ptr<ma_sound> m_ThemeSound;

		// /**
		//  * Plays the sound file given for a set time in milliseconds. Includes options
		//  * for volume and fade-in/fade-out length. Can be used to play segment of an
		//  * audio file or loop an audio file.
		//  *
		//  * @param entity - Entity to call the Timer Component on.
		//  * @param sfxName - The name of the sound file in /data/audio/sfx.
		//  * @param milliseconds - How long to play/loop the sound for in milliseconds.
		//  * @param volume - Volume to play the sound (Default=1.0f). This is also the maximum fade-in volume.
		//  * @param fadeInLengthInMilliseconds - Fade-in length in milliseconds (Default=0)
		//  * @param fadeOutLengthInMilliseconds - Fade-out length in milliseconds (Default=0)
		//  * @warning Do not include the file extension (e.g. ".mp3") in the sfxName! The method looks for .mp3
		//  *          files automatically!
		//  */
		// static void PlaySFXForMilliseconds(
		// 	const std::string& sfxName,
		// 	ma_uint64 milliseconds,
		// 	float volume,
		// 	ma_uint64 fadeInLengthInMilliseconds,
		// 	ma_uint64 fadeOutLengthInMilliseconds);

		// static void PlaySFXForMilliseconds(
		// 	const std::string& sfxName,
		// 	ma_uint64 milliseconds,
		// 	float volume);

		// static void PlaySFXForMilliseconds(
		// 	const std::string& sfxName,
		// 	ma_uint64 milliseconds);

		// /**
		//  * Plays the sound file given for a set time in seconds. Includes options
		//  * for volume and fade-in/fade-out length. Can be used to play segment of an
		//  * audio file or loop an audio file.
		//  *
		//  * @param sfxName - The name of the sound file in /data/audio/sfx.
		//  * @param seconds - How long to play/loop the sound for in seconds.
		//  * @param volume - Volume to play the sound (Default=1.0f). This is also the maximum fade-in volume.
		//  * @param fadeInLengthInSeconds - Fade-in length in seconds (Default=0)
		//  * @param fadeOutLengthInSeconds - Fade-out length in seconds (Default=0)
		//  * @warning Do not include the file extension (e.g. ".mp3") in the sfxName! The method looks for .mp3
		//  *          files automatically!
		//  */
		// static void PlaySFXForSeconds(
		// 	const std::string& sfxName,
		// 	ma_uint64 seconds,
		// 	float volume,
		// 	ma_uint64 fadeInLengthInSeconds,
		// 	ma_uint64 fadeOutLengthInSeconds);

		// static void PlaySFXForSeconds(
		// 	const std::string& sfxName,
		// 	ma_uint64 seconds,
		// 	float volume);

		// static void PlaySFXForSeconds(
		// 	const std::string& sfxName,
		// 	ma_uint64 seconds);

	private:
		static void timer(std::string sfxPath, ma_uint64 milliseconds, ma_sound* sound, float volume, ma_uint64 fadeOutLengthInMilliseconds);
		static std::unordered_set<std::string> _sounds_on_loop;
	};

} // namespace lei3d