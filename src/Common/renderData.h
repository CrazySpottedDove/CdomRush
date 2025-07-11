#pragma once

#include "Common/animation.h"
#include "Common/macros.h"
#include <SFML/Graphics/Texture.hpp>
#include <set>
#include <vector>

enum class RenderDataType
{
    Animation,   // 动画
    HealthBar,   // 血条
    Text,        // 文本
    TowerRange // 塔的范围
};

// Animations[0] 确定了指示状态，储存所有可以触发的点击事件
struct RenderData
{
    std::vector<Animation>* animations;
    Position                position;
    size_t                  layer_index;   // index 越大，越靠上显示
    double                  health_rate = 1.0;
    double range = 0.0;
    std::string             text;   // 用于文本显示
    RenderDataType            type;
    RenderData(std::vector<Animation>* animations, const Position& position,
             const size_t layer_index = COMMON_LAYER)
        : animations(animations)
        , position(position)
        , layer_index(layer_index)
        , type(RenderDataType::Animation)
    {}
    RenderData(const double health_rate, const Position& position)
        : animations(nullptr)
        , position(position)
        , layer_index(HEALTH_BAR_LAYER)
        , health_rate(health_rate)
        , type(RenderDataType::HealthBar)
    {}
    RenderData(const std::string& text, const Position& position, const size_t layer_index = 0)
        : animations(nullptr)
        , position(position)
        , layer_index(layer_index)
        , text(text)
        , type(RenderDataType::Text)
    {}
    RenderData(const double range, const Position& position, std::vector<Animation>* animations): animations(animations),
    position(position),layer_index(UI_LOWER_LAYER),type(RenderDataType::TowerRange), range(range){}
};

struct RenderDataComparator
{
    bool operator()(const RenderData& lhs, const RenderData& rhs) const
    {
        return lhs.layer_index < rhs.layer_index ||
               (lhs.position.y > rhs.position.y && lhs.layer_index == rhs.layer_index);
    }
};

typedef std::multiset<RenderData, RenderDataComparator> RenderDataQueue;