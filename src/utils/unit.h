#pragma once
class Store;
#include "states.h"
#include "vector2.h"
struct Armor
{
    double physical   = 0;
    double magical    = 0;
    double poison     = 0;
    double fire       = 0;
    double electrical = 0;
    double blood      = 0;
};

struct Health
{
    double hp_max;
    double hp;
    Armor  armor;
    // 尸体保持的时间
    double dead_lifetime = 3.0;
    // 是否免疫伤害
    bool immune = false;
};

struct Buff {
    double hp_factor = 1.0;
    double damage_factor = 1.0;
    double damage_inc = 0.0;
    double speed_factor = 1.0;
    double speed_inc = 0.0;
    double physical_armor_inc = 0.0;
    double magical_armor_inc = 0.0;
};

class Unit{
    Health health;
    Buff buff;
    double speed;
    virtual void Insert(Store& store) = 0;
    virtual void Update(Store& store) = 0;
    virtual void Remove(Store& store) = 0;
    Vector2 position;
    State state;
};