#pragma once
#include "ViewModel/GameViewModel/templates/unit.h"
#include "Common/macros.h"

class Soldier: public Unit{
public:
    sf::Vector2f slot; // 用于近战偏移
    Position rally_point;
};