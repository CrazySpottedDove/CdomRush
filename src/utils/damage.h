#pragma once
#include "unit.h"
#include <cstddef>
#include <memory>
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

struct Damage
{
    DamageType            type         = DamageType::Physical;
    double                value        = 0.0;
    double                ignore_armor = 0.0;
    std::shared_ptr<Unit> target;
    Damage(DamageType type, double value, double ignore_armor,
           std::shared_ptr<Unit> target) noexcept
        : type(type)
        , value(value)
        , ignore_armor(ignore_armor)
        , target(target)
    {}

    // TODO: 根据伤害类型结算伤害
    double ApplyDamage() const {}
};
