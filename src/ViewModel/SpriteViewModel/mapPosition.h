#pragma once

#include "Common/macros.h"

inline void MapPosition(Position& pos)
{
    constexpr float origin_screen_width   = 1024.0 * 8.4 / 6.4;
    const float origin_screen_height  = 768.0;
    pos.x                             = DEFAULT_SCREEN_WIDTH * pos.x / origin_screen_width;
    pos.y = DEFAULT_SCREEN_HEIGHT - (DEFAULT_SCREEN_HEIGHT * pos.y / origin_screen_height);
    pos.x += 225.0f;
    pos.y -= 35.0f;
}