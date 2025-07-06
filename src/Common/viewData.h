#pragma once

#include "Common/animation.h"
#include "Common/macros.h"
#include <SFML/Graphics/Texture.hpp>
#include <set>
#include <vector>

enum class ViewDataType
{
    Animation,   // 动画
    HealthBar,   // 血条
    Text,        // 文本
};

// Animations[0] 确定了指示状态，储存所有可以触发的点击事件
struct ViewData
{
    std::vector<Animation>* animations;
    Position                position;
    size_t                  layer_index;   // index 越大，越靠上显示
    double                  health_rate = 1.0;
    std::string             text;   // 用于文本显示
    ViewDataType            type;
    ViewData(std::vector<Animation>* animations, const Position& position,
             const size_t layer_index = COMMON_LAYER)
        : animations(animations)
        , position(position)
        , layer_index(layer_index)
        , type(ViewDataType::Animation)
    {}
    ViewData(const double health_rate, const Position& position)
        : animations(nullptr)
        , position(position)
        , layer_index(1)
        , health_rate(health_rate)
        , type(ViewDataType::HealthBar)
    {}
    ViewData(const std::string& text, const Position& position, const size_t layer_index = 0)
        : animations(nullptr)
        , position(position)
        , layer_index(layer_index)
        , text(text)
        , type(ViewDataType::Text)
    {}
};

struct ViewDataComparator
{
    bool operator()(const ViewData& lhs, const ViewData& rhs) const
    {
        return lhs.layer_index < rhs.layer_index ||
               (lhs.position.y > rhs.position.y && lhs.layer_index == rhs.layer_index);
    }
};

typedef std::multiset<ViewData, ViewDataComparator> ViewDataQueue;