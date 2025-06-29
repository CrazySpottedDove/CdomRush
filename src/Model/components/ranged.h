#pragma once
#include "Model/bullets/bullets.h"
#include "Model/components/attack.h"
#include <cstddef>
#include <vector>

class RangedAttack : public Attack
{
public:
    double  range;   // 射程
    BulletType bullet_type; // 弹道类型
    RangedAttack(double cooldown, double range, BulletType bullet_type_, double chance = 1.0)
        : Attack(cooldown, chance)
        , range(range)
        , bullet_type(bullet_type_) {};
};

class Ranged
{
public:
    std::vector<RangedAttack> attacks;
    RangedAttack&             operator[](const std::size_t idx) { return attacks[idx]; }
};