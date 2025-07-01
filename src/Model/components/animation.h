#pragma once
#include "Model/components/state.h"
#include <cstddef>
#include <string>
struct Animation
{
    Animation() = default;   // 默认构造函数
    Animation(const State state, const double rotation, const bool pending,
              const std::size_t frame_id, const std::string& prefix)
        : state(state)
        , rotation(rotation)
        , pending(pending)
        , frame_id(frame_id)
        , prefix(prefix) {};
    State       state;
    State       last_state;
    double      rotation = 0.0;    // 旋转角度
    bool        pending  = true;   // 动画正在进行
    bool        flip     = false;
    std::size_t frame_id = 0;   // 动画进行到第几帧
    std::string prefix;         // 动画的前缀名
};