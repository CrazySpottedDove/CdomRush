#pragma once
enum class State{
    // 正在行走
    Walking,

    // 敌人被拦截
    Blocked,

    // 死亡
    Dead,

    // 正在前往拦截
    Blocking,

    // 空闲
    Idle
};