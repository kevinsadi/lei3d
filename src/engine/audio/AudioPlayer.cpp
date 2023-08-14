#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include "audio/AudioPlayer.hpp"

namespace lei3d
{
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

        ma_engine_play_sound(m_AudioEngine.get(), "data/audio/breakcore.mp3", NULL);

        // WE NEED TO UNITIALIZE THE ENGINE AT SOME POINT
    }

}