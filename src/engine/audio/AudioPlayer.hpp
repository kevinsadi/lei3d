#pragma once

#include <memory>
#include "miniaudio.h"
#include "logging/Log.hpp"

namespace lei3d
{
    class AudioPlayer
    {
    public:
        AudioPlayer();

        std::unique_ptr<ma_engine> m_AudioEngine;
    };

}