#include "ViewModel/SpriteViewModel/stringMap.h"
#include "Common/state.h"

const std::unordered_map<std::string, TowerType> tower_type_str_map = {
    {"tower_holder_grass", TowerType::None},
    {"tower_archer_1", TowerType::Archer1},
    {"tower_archer_2", TowerType::Archer2},
    {"tower_archer_3", TowerType::Archer3},
    {"tower_engineer_1", TowerType::Engineer1},
    {"tower_engineer_2", TowerType::Engineer2},
    {"tower_engineer_3", TowerType::Engineer3},
    {"tower_mage_1", TowerType::Mage1},
    {"tower_mage_2", TowerType::Mage2},
    {"tower_mage_3", TowerType::Mage3}};

const std::unordered_map<std::string, EnemyType> enemy_type_str_map = {
    {"ForestTroll", EnemyType::ForestTroll}, {"OrcArmored", EnemyType::orc_armored}};

const std::unordered_map<std::string, State> state_str_map = {
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
    {"shootingDown", State::ShootingDown},
{"door_close",State::DoorClose},
{"door_open",State::DoorOpen},
{"enabled",State::Enabled},
{"disabled",State::Disabled},
{"none",State::None}};