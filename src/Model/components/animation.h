#pragma once
#include <cstddef>
#include <string>
#include "Model/components/state.h"
struct Animation{
    Animation() = default; // 默认构造函数
    State state;
    double rotation = 0.0; // 旋转角度
    bool pending = true; // 动画正在进行
    std::size_t frame_id = 0; // 动画进行到第几帧
    std::string prefix; // 动画的前缀名
};