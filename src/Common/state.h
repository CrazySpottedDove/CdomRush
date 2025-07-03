#pragma once
#include <string>
#include <unordered_map>
enum class State
{
    Idle,
    Walk,
    Attack,
    Shoot,
    Death,
    WalkingLeftRight,
    WalkingUp,
    WalkingDown,
    Running,
    IdleUp,
    IdleDown,
    Flying,
    Hit,
    ShootingUp,
    ShootingDown,
    None
};

extern const  std::unordered_map<std::string, State> state_str_map;