#pragma once
#include "../components/armor.h"
#include "../components/buff.h"
#include "../components/health.h"
#include "../components/state.h"
#include "entity.h"
#include "utils/macros.h"

// 拥有生命的实体
enum class UnitType
{
    Soldier,   // 士兵
    Enemy     // 敌人
};

enum class Heading
{
    Up,
    Down,
    Left,
    Right,
    None
};

class Unit : public Entity
{
public:
    UnitType     type; // 单位类型
    Health       health;
    Armor        armor;
    Buff         buff;
    double       speed;
    State        state;
    virtual bool OnDamage() { return true; };
    virtual ~Unit() = default;
};