#pragma once
#include "Model/bullets/bullets.h"
#include "Model/components/attack.h"
#include <cstddef>
#include <vector>

class Store;

class RangedAttack : public Attack
{
public:
    double value; // 伤害值
    double radius; // 爆炸半径
    double  range;   // 射程
    BulletType bullet_type; // 弹道类型
    Position bullet_start_offset; // 子弹起始偏移位置
    std::string prefix = "";
    RangedAttack(double cooldown, double range, BulletType bullet_type_, double radius_, double value_, std::string prefix_, double chance = 1.0)
        : Attack(cooldown, chance)
        , range(range)
        , bullet_type(bullet_type_)
        , radius(radius_)
        , value(value_)
        ,prefix(prefix_){};


    void Apply(Store& store, ID source, ID target, Position bullet_start_offset, SourceType source_type) noexcept;
};

class Ranged
{
public:
    std::vector<RangedAttack> attacks;
    RangedAttack&             operator[](const std::size_t idx) { return attacks[idx]; }
};