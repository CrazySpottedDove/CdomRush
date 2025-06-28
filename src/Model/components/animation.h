#pragma once
#include <cstddef>
#include <string>
#include "Model/components/state.h"
struct Animation{
    State state;
    double rotation;
    bool pending; // 动画正在进行
    std::size_t frame_id; // 动画进行到第几帧
    std::string prefix; // 动画的前缀名
};