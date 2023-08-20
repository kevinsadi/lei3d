#pragma once

#include <memory>
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
        void PlaySound(const std::string& sfxName) const;

        std::unique_ptr<ma_engine> m_AudioEngine;
    };

}