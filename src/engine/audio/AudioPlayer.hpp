#pragma once

#include <memory>
#include <ctime>
#include <thread>
#include <chrono>
#include <iostream>
#include "miniaudio.h"
#include "logging/Log.hpp"

namespace lei3d
{
    class AudioPlayer
    {
    public:
        static AudioPlayer* s_AudioPlayer;

        AudioPlayer();
        ~AudioPlayer();

        static AudioPlayer& GetAudioPlayer();
        
        static void PlaySFX(const std::string& sfxName);

        static void PlaySFXForMilliseconds(const std::string& sfxName, long milliseconds);

        std::unique_ptr<ma_engine> m_AudioEngine;

    private:
		static void dataCallback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);
    };

}