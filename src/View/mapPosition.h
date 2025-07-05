#pragma once

#include "Common/macros.h"

inline Position MapPosition(const Position& pos)
{
    return Position{
        WIDTH_FACTOR * pos.x + 225.0f,
        DEFAULT_SCREEN_HEIGHT - HEIGHT_FACTOR * pos.y
    };
}

inline Position MapPositionBack(const Position& pos)
{
    return Position{
        (pos.x - 225.0f) / WIDTH_FACTOR,
        (DEFAULT_SCREEN_HEIGHT - pos.y) / HEIGHT_FACTOR
    };
}