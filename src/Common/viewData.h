#pragma once

#include "Common/animation.h"
#include "Common/macros.h"
#include <SFML/Graphics/Texture.hpp>
#include <set>

struct ViewData{
    Animation* animation;
    Position position;
};

struct ViewDataComparator
{
    bool operator()(const ViewData& lhs, const ViewData& rhs) const
    {
        return lhs.position.y < rhs.position.y;
    }
};

typedef std::multiset<ViewData, ViewDataComparator> ViewDataQueue;