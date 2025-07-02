#pragma once
#include "Model/components/state.h"
#include <cstddef>
#include <string>
struct Animation
{
    Animation() = default;   // 默认构造函数
    Animation(const State state, const double rotation, const bool pending,
              const std::size_t frame_id, const std::string& prefix, const float anchor_x = 0.5f, const float anchor_y = 0.5f)
        : state(state)
        , rotation(rotation)
        , pending(pending)
        , frame_id(frame_id)
        , prefix(prefix), anchor_x(anchor_x),anchor_y(anchor_y) {};
    State       state;
    State       last_state = State::None;   // 上一个状态
    double      rotation   = 0.0;           // 旋转角度
    bool        pending    = true;          // 动画正在进行
    bool        flip       = false;
    float       anchor_x   = 0.5;
    float       anchor_y   = 0.5;
    std::size_t frame_id   = 1;   // 动画进行到第几帧
    std::string prefix;           // 动画的前缀名
};