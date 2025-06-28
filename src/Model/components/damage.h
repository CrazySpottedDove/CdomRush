#pragma once

#include "Model/templates/entity.h"
class Unit;
/**
 * @brief 伤害类型
 *
 */
enum class DamageType
{
    True           = 1,
    Physical       = 1 << 1,
    Magical        = 1 << 2,
    Explosion      = 1 << 3,
    Poison         = 1 << 4,
    Fire           = 1 << 5,
    Electrical     = 1 << 6,
    Tear           = 1 << 7,
    Blood          = 1 << 8,
    Physical_Armor = 1 << 9,    // 对物理护甲造成伤害
    Magical_Armor  = 1 << 10,   // 对魔法护甲造成伤害
};

struct DamageData
{
    DamageType type         = DamageType::Physical;
    double     value        = 0.0;
    double     ignore_armor = 0.0;
    int        apply_delay  = 0;   // 延迟多少个帧造成伤害
    DamageData(const double value, const DamageType type = DamageType::Physical,
               const double ignore_armor = 0.0, const int apply_delay = 0)
        : type(type)
        , value(value)
        , ignore_armor(ignore_armor)
        , apply_delay(apply_delay)
    {}
    DamageData(const DamageData& other) = default;
    DamageData& operator=(const DamageData& other) = default;
};

struct DamageEvent
{
    DamageData data;
    Entity*      target      = nullptr;
    Entity*      source      = nullptr;

    DamageEvent(const DamageData& data, Entity* target, Entity* source)
        : data(data)
        , source(source)
        , target(target)
        {};
    
};
