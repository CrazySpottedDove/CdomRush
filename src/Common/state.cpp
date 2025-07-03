#include "Common/state.h"

const  std::unordered_map<std::string, State> state_str_map = {
    {"attack", State::Attack},
    {"idle", State::Idle},
    {"walk", State::Walk},
    {"shoot", State::Shoot},
    {"death", State::Death},
    {"walkingLeftRight", State::WalkingLeftRight},
    {"walkingUp", State::WalkingUp},
    {"walkingDown", State::WalkingDown},
    {"running", State::Running},
    {"idleUp", State::IdleUp},
    {"idleDown", State::IdleDown},
    {"flying", State::Flying},
    {"hit", State::Hit},
    {"shootingUp", State::ShootingUp},
    {"shootingDown", State::ShootingDown}};