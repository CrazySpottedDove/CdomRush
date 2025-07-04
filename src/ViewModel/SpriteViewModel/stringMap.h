#pragma once
#include "Common/state.h"
#include "Common/type.h"
#include <string>
#include <unordered_map>

extern const std::unordered_map<std::string, State> state_str_map;

extern const std::unordered_map<std::string, TowerType> tower_type_str_map;

extern const std::unordered_map<std::string, EnemyType> enemy_type_str_map;