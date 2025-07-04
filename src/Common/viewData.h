#pragma once

#include "Common/animation.h"
#include "Common/macros.h"
#include <SFML/Graphics/Texture.hpp>
#include <set>

// Animations[0] 确定了指示状态，储存所有可以触发的点击事件
struct ViewData{
    std::vector<Animation>* animations;
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