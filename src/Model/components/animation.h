#pragma once
#include <cstddef>
#include <string>
#include "Model/components/state.h"
struct Animation{
    Animation(State state_, double rotation_ = 0.0, bool pending_ = false, std::size_t frame_id_ = 0, const std::string& prefix_ = "")
        : state(state_)
        , rotation(rotation_)
        , pending(pending_)
        , frame_id(frame_id_)
        , prefix(prefix_) {}
    
    Animation() = default; // 默认构造函数
    State state;
    double rotation;
    bool pending; // 动画正在进行
    std::size_t frame_id; // 动画进行到第几帧
    std::string prefix; // 动画的前缀名
};