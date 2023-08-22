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
        if (result != MA_SUCCESS) {
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

    void AudioPlayer::PlaySFX(const std::string& sfxName)
    {
        std::string sfxPath = "data/audio/sfx/" + sfxName + ".mp3";
        ma_engine_play_sound(s_AudioPlayer->m_AudioEngine.get(), sfxPath.c_str(), NULL);
    }
}