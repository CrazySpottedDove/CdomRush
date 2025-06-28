#pragma once
#include "Model/bullets/bullets.h"
#include "Model/components/attack.h"
#include <cstddef>
#include <vector>

class RangedAttack : public Attack
{
public:
    double  range;   // 射程
    Bullet* bullet;
    RangedAttack(double cooldown, double range, Bullet* bullet, double chance = 1.0)
        : Attack(cooldown, chance)
        , range(range)
        , bullet(bullet) {};
    ~RangedAttack()
    {
        delete bullet;
        bullet = nullptr;
    }
};

class Ranged
{
public:
    std::vector<RangedAttack> attacks;
    RangedAttack&             operator[](const std::size_t idx) { return attacks[idx]; }
};