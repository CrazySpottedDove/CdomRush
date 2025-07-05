#pragma once

#include "Common/macros.h"

inline Position MapPosition(const Position& pos)
{
    static constexpr float origin_screen_width   = 1024.0 * 8.4 / 6.4;
    static const float origin_screen_height  = 768.0;
    return Position{
        DEFAULT_SCREEN_WIDTH* pos.x / origin_screen_width + 225.0f,
        DEFAULT_SCREEN_HEIGHT - (DEFAULT_SCREEN_HEIGHT * pos.y / origin_screen_height)
    };
}